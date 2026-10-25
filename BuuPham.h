// Nguyễn Như Khánh
#ifndef BUUPHAM_H
#define BUUPHAM_H

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <stdexcept>
#include <functional>
#include "Entity.h"
#include "Repository.h"
#include "BuuCuc.h"
#include "HoaDonCuoc.h"

using namespace std;

class BuuPham : public Entity {
private:
    string tenBuuPham;
    double trongLuong;
    string maBuuCucGui;
    string maHoaDon; // Khóa ngoại tham chiếu sang Hóa đơn

public:
    BuuPham() : Entity(), tenBuuPham(""), trongLuong(0.0), maBuuCucGui(""), maHoaDon("") {}

    string getTenBuuPham() const { return tenBuuPham; }
    void setTenBuuPham(const string& ten) {
        if (ten.empty()) throw invalid_argument("Ten buu pham khong duoc de trong!");
        tenBuuPham = ten;
    }

    double getTrongLuong() const { return trongLuong; }
    void setTrongLuong(double tl) {
        if (tl <= 0) throw invalid_argument("Trong luong phai lon hon 0!");
        trongLuong = tl;
    }

    string getMaBuuCucGui() const { return maBuuCucGui; }
    void setMaBuuCucGui(const string& maBC) {
        if (maBC.empty()) throw invalid_argument("Ma buu cuc gui khong duoc de trong!");
        maBuuCucGui = maBC;
    }

    string getMaHoaDon() const { return maHoaDon; }
    void setMaHoaDon(const string& maHD) {
        if (maHD.empty()) throw invalid_argument("Ma hoa don khong duoc de trong!");
        maHoaDon = maHD;
    }

    void nhap() override {
        string temp;
        while (true) {
            try {
                cout << "Nhap ten buu pham: "; getline(cin, temp);
                setTenBuuPham(temp); break;
            }
            catch (const exception& e) { cout << "Loi: " << e.what() << " Vui long nhap lai.\n"; }
        }
        while (true) {
            try {
                cout << "Nhap trong luong (kg): "; cin >> trongLuong;
                if (cin.fail() || trongLuong <= 0) {
                    cin.clear(); cin.ignore(10000, '\n');
                    throw invalid_argument("Trong luong phai la so thuc lon hon 0!");
                }
                cin.ignore(); break;
            }
            catch (const exception& e) { cout << "Loi: " << e.what() << "\n"; }
        }
    }

    void xuat() const override {
        cout << left << setw(8) << maID << "| " << setw(18) << tenBuuPham
             << "| " << setw(12) << fixed << setprecision(1) << trongLuong 
             << "| " << setw(10) << maBuuCucGui 
             << "| " << setw(10) << maHoaDon << endl;
    }

    string toFileRecord() const override {
        stringstream ss;
        ss << maID << "|" << tenBuuPham << "|" << trongLuong << "|" << maBuuCucGui << "|" << maHoaDon;
        return ss.str();
    }

    void fromFileRecord(const string& record) override {
        stringstream ss(record);
        string item; vector<string> tokens;
        while (getline(ss, item, '|')) tokens.push_back(item);
        if (tokens.size() >= 5) {
            maID = tokens[0];
            tenBuuPham = tokens[1];
            trongLuong = stod(tokens[2]);
            maBuuCucGui = tokens[3];
            maHoaDon = tokens[4];
        }
    }
};

class BuuPhamMenu {
private:
    Repository<BuuPham>& repoBP;
    Repository<BuuCuc>& repoBC;
    Repository<HoaDonCuoc>& repoHD; 
    
    function<bool(string)> kiemTraRangBuocPhieuGui;

    void xoaManHinh() const { system("cls"); }
    void tamDung() const { cout << "\nNhan Enter de tiep tuc..."; cin.get(); }
    void inTieuDeBang() const {
        cout << left << setw(8) << "Ma BP" << "| " << setw(18) << "Ten Buu Pham"
             << "| " << setw(12) << "Trong Luong" << "| " << setw(10) << "Ma BC" << "| " << setw(10) << "Ma HD" << endl;
        cout << string(64, '-') << endl;
    }

public:
    BuuPhamMenu(Repository<BuuPham>& bp, Repository<BuuCuc>& bc, Repository<HoaDonCuoc>& hd, function<bool(string)> kiemTraPG = nullptr) 
        : repoBP(bp), repoBC(bc), repoHD(hd), kiemTraRangBuocPhieuGui(kiemTraPG) {}

    void themMoi() {
        cout << "\n=== THEM MOI BUU PHAM ===\n";
        BuuPham bp; string temp;

        while (true) {
            try {
                cout << "Nhap ma buu pham (VD: BP01): "; getline(cin, temp);
                bp.setMaID(temp);
                if (repoBP.findById(temp) != nullptr) throw invalid_argument("Ma buu pham da ton tai!");
                break;
            }
            catch (const exception& e) { cout << "Loi: " << e.what() << " Vui long nhap lai.\n"; }
        }

        while (true) {
            try {
                cout << "Nhap ma Buu cuc gui (VD: BC01): "; getline(cin, temp);
                if (temp.empty()) throw invalid_argument("Khong duoc de trong!");
                if (repoBC.findById(temp) == nullptr) {
                    throw invalid_argument("Buu cuc nay chua ton tai trong he thong!");
                }
                bp.setMaBuuCucGui(temp);
                break;
            }
            catch (const exception& e) { cout << "Loi: " << e.what() << "\n"; }
        }

        while (true) {
            try {
                cout << "Nhap ma Hoa don cuoc (VD: HD01): "; getline(cin, temp);
                if (temp.empty()) throw invalid_argument("Khong duoc de trong!");
                
                // 1. Kiểm tra xem hóa đơn có tồn tại trong hệ thống hay không
                if (repoHD.findById(temp) == nullptr) {
                    throw invalid_argument("Hoa don nay chua ton tai trong he thong!");
                }

                // 2. Kiểm tra xem mã hóa đơn này đã được gán cho bưu phẩm nào khác chưa (tránh trùng lặp 1-1)
                bool daDung = false;
                for (const auto& item : repoBP.getAll()) {
                    if (item.getMaHoaDon() == temp) {
                        daDung = true;
                        break;
                    }
                }
                if (daDung) {
                    throw invalid_argument("Ma hoa don nay da duoc su dung cho mot buu pham khac!");
                }

                bp.setMaHoaDon(temp);
                break;
            }
            catch (const exception& e) { cout << "Loi: " << e.what() << "\n"; }
        }

        bp.nhap();
        repoBP.add(bp);
        cout << "-> Them moi buu pham thanh cong!\n";
    }

    void xemDanhSach() const {
        auto list = repoBP.getAll();
        if (list.empty()) { cout << "\nDanh sach buu pham trong!\n"; return; }
        cout << "\n============================= DANH SACH BUU PHAM =============================\n";
        inTieuDeBang();
        for (const auto& item : list) item.xuat();
        cout << "Tong so buu pham: " << list.size() << endl;
    }

    void timKiem() const {
        string id; cout << "Nhap ma buu pham can tim: "; getline(cin, id);
        BuuPham* result = repoBP.findById(id);
        if (result != nullptr) {
            cout << "\nDa tim thay thong tin:\n"; inTieuDeBang(); result->xuat();
        } else {
            cout << "Khong tim thay buu pham co ma: " << id << "\n";
        }
    }

    void sapXep() {
        auto list = repoBP.getAll();
        if (list.empty()) { cout << "\nDanh sach trong!\n"; return; }

        sort(list.begin(), list.end(), [](const BuuPham& a, const BuuPham& b) {
            return a.getTrongLuong() < b.getTrongLuong();
        });

        cout << "-> Da sap xep danh sach theo Trong Luong (Tang dan)!\n";
        cout << "\n============================= KET QUA SAP XEP =============================\n";
        inTieuDeBang();
        for (const auto& item : list) item.xuat();
    }

    void capNhat() {
        string id; cout << "Nhap ma buu pham can cap nhat: "; getline(cin, id);
        BuuPham* old = repoBP.findById(id);
        if (old == nullptr) { cout << "Khong tim thay ma buu pham nay!\n"; return; }

        cout << "\nThong tin hien tai:\n"; inTieuDeBang(); old->xuat();
        cout << "\n(Luu y: An Enter de giu nguyen gia tri cu)\n";

        BuuPham newData = *old;
        string temp;

        cout << "Nhap ten moi [" << old->getTenBuuPham() << "]: "; getline(cin, temp);
        if (!temp.empty()) newData.setTenBuuPham(temp);

        cout << "Nhap trong luong moi [" << old->getTrongLuong() << "]: "; getline(cin, temp);
        if (!temp.empty()) newData.setTrongLuong(stod(temp));

        cout << "Nhap ma buu cuc moi [" << old->getMaBuuCucGui() << "]: "; getline(cin, temp);
        if (!temp.empty()) {
            if (repoBC.findById(temp) == nullptr) {
                cout << "Loi: Buu cuc khong ton tai! Giu nguyen ma cu.\n";
            } else {
                newData.setMaBuuCucGui(temp);
            }
        }

        cout << "Nhap ma hoa don moi [" << old->getMaHoaDon() << "]: "; getline(cin, temp);
        if (!temp.empty()) {
            if (repoHD.findById(temp) == nullptr) {
                cout << "Loi: Hoa don khong ton tai! Giu nguyen ma cu.\n";
            } else {
                // Kiểm tra trùng lặp mã hóa đơn (trừ chính bưu phẩm đang cập nhật)
                bool daDung = false;
                for (const auto& item : repoBP.getAll()) {
                    if (item.getMaID() != id && item.getMaHoaDon() == temp) {
                        daDung = true;
                        break;
                    }
                }
                if (daDung) {
                    cout << "Loi: Ma hoa don nay da duoc su dung cho buu pham khac! Giu nguyen ma cu.\n";
                } else {
                    newData.setMaHoaDon(temp);
                }
            }
        }

        if (repoBP.update(id, newData)) cout << "-> Cap nhat thanh cong!\n";
        else cout << "Loi khi cap nhat!\n";
    }

    void xoa() {
        string id; cout << "Nhap ma buu pham can xoa: "; getline(cin, id);
        BuuPham* target = repoBP.findById(id);
        if (target == nullptr) { cout << "Khong tim thay ma buu pham nay!\n"; return; }

        cout << "\nThong tin chuan bi xoa:\n"; inTieuDeBang(); target->xuat();
        
        if (kiemTraRangBuocPhieuGui != nullptr && kiemTraRangBuocPhieuGui(id)) {
            cout << "\n[LOI RANG BUOC DU LIEU] KHONG THE XOA!\n";
            cout << "-> Buu pham nay dang duoc tham chiếu trong Phieu gui.\n";
            return;
        }

        cout << "Ban co chac chan muon xoa khong? (y/n): ";
        char confirm; cin >> confirm; cin.ignore(10000, '\n');

        if (confirm == 'y' || confirm == 'Y') {
            if (repoBP.removeById(id)) cout << "-> Xoa thanh cong!\n";
        } else {
            cout << "Huy thao tac xoa.\n";
        }
    }

    void menu() {
        int choice = -1;
        do {
            xoaManHinh();
            cout << "========== QUAN LY BUU PHAM ==========\n";
            cout << "1. Them buu pham moi\n";
            cout << "2. Xem danh sach\n";
            cout << "3. Tim kiem theo ma\n";
            cout << "4. Sap xep theo Trong Luong\n";
            cout << "5. Cap nhat thong tin\n";
            cout << "6. Xoa buu pham\n";
            cout << "0. Quay lai menu chinh\n";
            cout << "=======================================\nLua chon cua ban: ";
            if (!(cin >> choice)) { cin.clear(); choice = -1; }
            cin.ignore(10000, '\n');
            if (choice == 0) break;

            xoaManHinh();
            switch (choice) {
            case 1: themMoi(); break;
            case 2: xemDanhSach(); break;
            case 3: timKiem(); break;
            case 4: sapXep(); break;
            case 5: capNhat(); break;
            case 6: xoa(); break;
            default: cout << "Lua chon khong hop le!\n";
            }
            tamDung();
        } while (choice != 0);
    }
};

#endif
