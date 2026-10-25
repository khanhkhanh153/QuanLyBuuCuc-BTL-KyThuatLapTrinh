// Nguyễn Như Khánh
#ifndef LOAIDICHVU_H
#define LOAIDICHVU_H

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

using namespace std;

class LoaiDichVu : public Entity {
private:
    string tenDichVu;
    double phuPhi;
    string moTa;

public:
    LoaiDichVu() : Entity(), tenDichVu(""), phuPhi(0.0), moTa("") {}

    string getTenDichVu() const { return tenDichVu; }
    void setTenDichVu(const string& ten) {
        if (ten.empty()) throw invalid_argument("Ten dich vu khong duoc de trong!");
        tenDichVu = ten;
    }

    double getPhuPhi() const { return phuPhi; }
    void setPhuPhi(double pp) {
        if (pp < 0) throw invalid_argument("Phu phi khong duoc am!");
        phuPhi = pp;
    }

    string getMoTa() const { return moTa; }
    void setMoTa(const string& mt) {
        moTa = mt;
    }

    void nhap() override {
        string temp;
        while (true) {
            try {
                cout << "Nhap ten loai dich vu: "; getline(cin, temp);
                setTenDichVu(temp); break;
            }
            catch (const exception& e) { cout << "Loi: " << e.what() << " Vui long nhap lai.\n"; }
        }
        while (true) {
            try {
                cout << "Nhap muc phu phi (VND): "; cin >> phuPhi;
                if (cin.fail() || phuPhi < 0) {
                    cin.clear(); cin.ignore(10000, '\n');
                    throw invalid_argument("Phu phi phai la so khong am!");
                }
                cin.ignore(); break;
            }
            catch (const exception& e) { cout << "Loi: " << e.what() << "\n"; }
        }
        cout << "Nhap mo ta dich vu: "; getline(cin, moTa);
    }

    void xuat() const override {
        cout << left << setw(10) << maID << "| " << setw(25) << tenDichVu
            << "| " << setw(15) << fixed << setprecision(0) << phuPhi
            << "| " << setw(25) << moTa << endl;
    }

    string toFileRecord() const override {
        stringstream ss;
        ss << maID << "|" << tenDichVu << "|" << phuPhi << "|" << moTa;
        return ss.str();
    }

    void fromFileRecord(const string& record) override {
        stringstream ss(record);
        string item; vector<string> tokens;
        while (getline(ss, item, '|')) tokens.push_back(item);
        if (tokens.size() >= 3) {
            maID = tokens[0];
            tenDichVu = tokens[1];
            phuPhi = stod(tokens[2]);
            if (tokens.size() >= 4) moTa = tokens[3];
        }
    }
};

class LoaiDichVuMenu {
private:
    Repository<LoaiDichVu>& repo;
    
    // Callback kiểm tra ràng buộc chéo (ví dụ: với Hóa đơn theo sơ đồ mới)
    function<bool(string)> kiemTraRangBuocHD;

    void xoaManHinh() const { system("cls"); }
    void tamDung() const { cout << "\nNhan Enter de tiep tuc..."; cin.get(); }
    void inTieuDeBang() const {
        cout << left << setw(10) << "Ma DV" << "| " << setw(25) << "Ten Loai Dich Vu"
            << "| " << setw(15) << "Phu Phi" << "| " << setw(25) << "Mo Ta" << endl;
        cout << string(73, '-') << endl;
    }

public:
    LoaiDichVuMenu(Repository<LoaiDichVu>& r, function<bool(string)> kiemTra = nullptr) 
        : repo(r), kiemTraRangBuocHD(kiemTra) {}

    void themMoi() {
        cout << "\n=== THEM MOI LOAI DICH VU ===\n";
        LoaiDichVu dv; string id;
        while (true) {
            try {
                cout << "Nhap ma loai dich vu (VD: DV01): "; getline(cin, id);
                dv.setMaID(id);
                if (repo.findById(id) != nullptr) throw invalid_argument("Ma loai dich vu da ton tai!");
                break;
            }
            catch (const exception& e) { cout << "Loi: " << e.what() << " Vui long nhap lai.\n"; }
        }
        dv.nhap();
        repo.add(dv);
        cout << "-> Them moi loai dich vu thanh cong!\n";
    }

    void xemDanhSach() const {
        auto list = repo.getAll();
        if (list.empty()) { cout << "\nDanh sach loai dich vu trong!\n"; return; }
        cout << "\n======================== DANH SACH LOAI DICH VU ========================\n";
        inTieuDeBang();
        for (const auto& item : list) item.xuat();
        cout << "Tong so loai dich vu: " << list.size() << endl;
    }

    void timKiem() const {
        string id; cout << "Nhap ma loai dich vu can tim: "; getline(cin, id);
        LoaiDichVu* result = repo.findById(id);
        if (result != nullptr) {
            cout << "\nDa tim thay thong tin:\n"; inTieuDeBang(); result->xuat();
        }
        else {
            cout << "Khong tim thay loai dich vu co ma: " << id << "\n";
        }
    }

    void sapXep() {
        auto list = repo.getAll();
        if (list.empty()) { cout << "\nDanh sach trong!\n"; return; }

        sort(list.begin(), list.end(), [](const LoaiDichVu& a, const LoaiDichVu& b) {
            return a.getTenDichVu() < b.getTenDichVu();
            });

        cout << "-> Da sap xep danh sach theo Ten Dich Vu (A-Z)!\n";
        cout << "\n======================== KET QUA SAP XEP ========================\n";
        inTieuDeBang();
        for (const auto& item : list) item.xuat();
    }

    void capNhat() {
        string id; cout << "Nhap ma loai dich vu can cap nhat: "; getline(cin, id);
        LoaiDichVu* old = repo.findById(id);
        if (old == nullptr) { cout << "Khong tim thay ma loai dich vu nay!\n"; return; }

        cout << "\nThong tin hien tai:\n"; inTieuDeBang(); old->xuat();
        cout << "\n(Luu y: An Enter de giu nguyen gia tri cu)\n";

        LoaiDichVu newData = *old;
        string temp;

        cout << "Nhap ten moi [" << old->getTenDichVu() << "]: "; getline(cin, temp);
        if (!temp.empty()) newData.setTenDichVu(temp);

        cout << "Nhap phu phi moi [" << old->getPhuPhi() << "]: "; getline(cin, temp);
        if (!temp.empty()) newData.setPhuPhi(stod(temp));

        cout << "Nhap mo ta moi [" << old->getMoTa() << "]: "; getline(cin, temp);
        if (!temp.empty()) newData.setMoTa(temp);

        if (repo.update(id, newData)) cout << "-> Cap nhat thanh cong!\n";
        else cout << "Loi khi cap nhat!\n";
    }

    void xoa() {
        string id; cout << "Nhap ma loai dich vu can xoa: "; getline(cin, id);
        LoaiDichVu* target = repo.findById(id);
        if (target == nullptr) { cout << "Khong tim thay ma loai dich vu nay!\n"; return; }

        cout << "\nThong tin chuan bi xoa:\n"; inTieuDeBang(); target->xuat();
        
        // Kiểm tra ràng buộc tham chiếu trước khi xóa
        if (kiemTraRangBuocHD != nullptr && kiemTraRangBuocHD(id)) {
            cout << "\n[LOI RANG BUOC DU LIEU] KHONG THE XOA!\n";
            cout << "-> Loai dich vu nay dang duoc su dung trong hoa don.\n";
            return;
        }

        cout << "Ban co chac chan muon xoa khong? (y/n): ";
        char confirm; cin >> confirm; cin.ignore(10000, '\n');

        if (confirm == 'y' || confirm == 'Y') {
            if (repo.removeById(id)) cout << "-> Xoa thanh cong!\n";
        }
        else {
            cout << "Huy thao tac xoa.\n";
        }
    }

    void menu() {
        int choice = -1;
        do {
            xoaManHinh();
            cout << "========== QUAN LY LOAI DICH VU ==========\n";
            cout << "1. Them loai dich vu moi\n";
            cout << "2. Xem danh sach\n";
            cout << "3. Tim kiem theo ma\n";
            cout << "4. Sap xep theo Ten (A-Z)\n";
            cout << "5. Cap nhat thong tin\n";
            cout << "6. Xoa loai dich vu\n";
            cout << "0. Quay lai menu chinh\n";
            cout << "=========================================\nLua chon cua ban: ";
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
