#ifndef HOADONCUOC_H
#define HOADONCUOC_H

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <stdexcept>
#include <cstdlib>
#include "Entity.h"
#include "Repository.h"
#include "TuyenVanChuyen.h"

using namespace std;

// --- LOP THUC THE HOADONCUOC ---
class HoaDonCuoc : public Entity {
private:
    string maTuyen;
    double cuocChinh;
    double phuPhi;
    string trangThaiThanhToan; // "Da thanh toan" hoac "Chua thanh toan"

public:
    HoaDonCuoc() : Entity(), maTuyen(""), cuocChinh(0.0), phuPhi(0.0), trangThaiThanhToan("Chua thanh toan") {}

    string getMaTuyen() const { return maTuyen; }
    void setMaTuyen(const string& maT) {
        if (maT.empty()) throw invalid_argument("Ma tuyen van chuyen khong duoc de trong!");
        maTuyen = maT;
    }

    double getCuocChinh() const { return cuocChinh; }
    void setCuocChinh(double cc) {
        if (cc < 0) throw invalid_argument("Cuoc chinh khong duoc am!");
        cuocChinh = cc;
    }

    double getPhuPhi() const { return phuPhi; }
    void setPhuPhi(double pp) {
        if (pp < 0) throw invalid_argument("Phu phi khong duoc am!");
        phuPhi = pp;
    }

    double getTongTien() const {
        return cuocChinh + phuPhi;
    }

    string getTrangThaiThanhToan() const { return trangThaiThanhToan; }
    void setTrangThaiThanhToan(const string& tt) {
        if (tt.empty()) throw invalid_argument("Trang thai thanh toan khong duoc de trong!");
        trangThaiThanhToan = tt;
    }

    void nhap() override {
        while (true) {
            try {
                cout << "Nhap cuoc chinh (VND): ";
                cin >> cuocChinh;
                if (cin.fail() || cuocChinh < 0) {
                    cin.clear(); cin.ignore(10000, '\n');
                    throw invalid_argument("Cuoc chinh phai la so khong am!");
                }
                cin.ignore(10000, '\n');
                break;
            }
            catch (const exception& e) {
                cout << "Loi: " << e.what() << " Vui long nhap lai.\n";
            }
        }

        while (true) {
            try {
                cout << "Nhap phu phi (VND): ";
                cin >> phuPhi;
                if (cin.fail() || phuPhi < 0) {
                    cin.clear(); cin.ignore(10000, '\n');
                    throw invalid_argument("Phu phi phai la so khong am!");
                }
                cin.ignore(10000, '\n');
                break;
            }
            catch (const exception& e) {
                cout << "Loi: " << e.what() << " Vui long nhap lai.\n";
            }
        }

        cout << "Chon trang thai thanh toan (1 - Da thanh toan, 2 - Chua thanh toan) [Default: 2]: ";
        string input;
        getline(cin, input);
        if (input == "1") trangThaiThanhToan = "Da thanh toan";
        else trangThaiThanhToan = "Chua thanh toan";
    }

    void xuat() const override {
        cout << left << setw(12) << maID
            << "| " << setw(15) << maTuyen
            << "| " << setw(15) << fixed << setprecision(0) << cuocChinh
            << "| " << setw(12) << fixed << setprecision(0) << phuPhi
            << "| " << setw(15) << fixed << setprecision(0) << getTongTien()
            << "| " << setw(18) << trangThaiThanhToan << endl;
    }

    string toFileRecord() const override {
        stringstream ss;
        ss << maID << "|" << maTuyen << "|" << cuocChinh << "|" << phuPhi << "|" << trangThaiThanhToan;
        return ss.str();
    }

    void fromFileRecord(const string& record) override {
        stringstream ss(record);
        string item;
        vector<string> tokens;
        while (getline(ss, item, '|')) tokens.push_back(item);
        if (tokens.size() == 5) {
            maID = tokens[0];
            maTuyen = tokens[1];
            cuocChinh = stod(tokens[2]);
            phuPhi = stod(tokens[3]);
            trangThaiThanhToan = tokens[4];
        }
    }
};

// --- LOP GIAO DIEN (MENU HOADONCUOC) ---
class HoaDonCuocMenu {
private:
    Repository<HoaDonCuoc>& repoHD;
    Repository<TuyenVanChuyen>& repoTVC;

    void xoaManHinh() const { system("cls"); }
    void tamDung() const { cout << "\nNhan Enter de tiep tuc..."; cin.get(); }

    void inTieuDeBang() const {
        cout << left << setw(12) << "Ma Hoa Don"
            << "| " << setw(15) << "Ma Tuyen"
            << "| " << setw(15) << "Cuoc Chinh(VND)"
            << "| " << setw(12) << "Phu Phi(VND)"
            << "| " << setw(15) << "Tong Tien(VND)"
            << "| " << setw(18) << "Trang Thai TT" << endl;
        cout << string(93, '-') << endl;
    }

public:
    HoaDonCuocMenu(Repository<HoaDonCuoc>& hd, Repository<TuyenVanChuyen>& tvc)
        : repoHD(hd), repoTVC(tvc) {
    }

    void themMoi() {
        cout << "\n=== THEM MOI HOA DON CUOC ===\n";
        HoaDonCuoc hd;
        string temp;

        while (true) {
            try {
                cout << "Nhap ma hoa don (VD: HD01): ";
                getline(cin, temp);
                hd.setMaID(temp);
                if (repoHD.findById(temp) != nullptr) throw invalid_argument("Ma hoa don da ton tai!");
                break;
            }
            catch (const exception& e) {
                cout << "Loi: " << e.what() << " Vui long nhap lai.\n";
            }
        }

        // Kiem tra tham chieu sang TuyenVanChuyen
        while (true) {
            try {
                cout << "Nhap ma Tuyen van chuyen (VD: TVC01): ";
                getline(cin, temp);
                if (temp.empty()) throw invalid_argument("Khong duoc de trong!");
                if (repoTVC.findById(temp) == nullptr) {
                    throw invalid_argument("Tuyen van chuyen nay chua ton tai! Vui long kiem tra lai.");
                }
                hd.setMaTuyen(temp);
                break;
            }
            catch (const exception& e) {
                cout << "Loi: " << e.what() << "\n";
            }
        }

        hd.nhap();
        repoHD.add(hd);
        cout << "-> Them moi hoa don thanh cong!\n";
    }

    void xemDanhSach() const {
        auto list = repoHD.getAll();
        if (list.empty()) {
            cout << "\nDanh sach hoa don trong!\n";
            return;
        }
        cout << "\n=================================== DANH SACH HOA DON CUOC ===================================\n";
        inTieuDeBang();
        for (const auto& item : list) item.xuat();
        cout << "Tong so hoa don: " << list.size() << endl;
    }

    void timKiem() const {
        string id;
        cout << "Nhap ma hoa don can tim: ";
        getline(cin, id);
        HoaDonCuoc* result = repoHD.findById(id);
        if (result != nullptr) {
            cout << "\nDa tim thay thong tin:\n";
            inTieuDeBang();
            result->xuat();
        }
        else cout << "Khong tim thay hoa don ma: " << id << "\n";
    }

    void sapXep() {
        auto list = repoHD.getAll();
        if (list.empty()) {
            cout << "\nDanh sach trong!\n";
            return;
        }

        sort(list.begin(), list.end(), [](const HoaDonCuoc& a, const HoaDonCuoc& b) {
            return a.getTongTien() > b.getTongTien();
            });

        cout << "-> Da sap xep danh sach hien thi theo Tong Tien (Giam dan)!\n";
        cout << "\n=================================== KET QUA SAP XEP ===================================\n";
        inTieuDeBang();
        for (const auto& item : list) item.xuat();
    }

    void capNhat() {
        string id;
        cout << "Nhap ma hoa don can cap nhat: ";
        getline(cin, id);
        HoaDonCuoc* old = repoHD.findById(id);
        if (old == nullptr) {
            cout << "Khong tim thay ma nay!\n";
            return;
        }

        cout << "\nThong tin hien tai:\n";
        inTieuDeBang();
        old->xuat();
        cout << "\n(Luu y: De trong hoac nhap -1 neu muon giu nguyen gia tri cu)\n";

        HoaDonCuoc newData = *old;
        string temp;

        cout << "Nhap ma Tuyen van chuyen moi [" << old->getMaTuyen() << "]: ";
        getline(cin, temp);
        if (!temp.empty()) {
            if (repoTVC.findById(temp) == nullptr) cout << "Loi: Tuyen van chuyen khong ton tai! Giu nguyen ma cu.\n";
            else newData.setMaTuyen(temp);
        }

        try {
            cout << "Nhap cuoc chinh moi [" << fixed << setprecision(0) << old->getCuocChinh() << "]: ";
            getline(cin, temp);
            if (!temp.empty() && stod(temp) != -1) newData.setCuocChinh(stod(temp));
        }
        catch (...) {}

        try {
            cout << "Nhap phu phi moi [" << fixed << setprecision(0) << old->getPhuPhi() << "]: ";
            getline(cin, temp);
            if (!temp.empty() && stod(temp) != -1) newData.setPhuPhi(stod(temp));
        }
        catch (...) {}

        cout << "Nhap trang thai thanh toan moi (1 - Da thanh toan, 2 - Chua thanh toan) [" << old->getTrangThaiThanhToan() << "]: ";
        getline(cin, temp);
        if (temp == "1") newData.setTrangThaiThanhToan("Da thanh toan");
        else if (temp == "2") newData.setTrangThaiThanhToan("Chua thanh toan");

        if (repoHD.update(id, newData)) cout << "-> Cap nhat thanh cong!\n";
        else cout << "Loi khi cap nhat!\n";
    }

    void xoa() {
        string id;
        cout << "Nhap ma hoa don can xoa: ";
        getline(cin, id);
        HoaDonCuoc* target = repoHD.findById(id);
        if (target == nullptr) {
            cout << "Khong tim thay ma nay!\n";
            return;
        }

        cout << "\nThong tin chuan bi xoa:\n";
        inTieuDeBang();
        target->xuat();
        cout << "Ban co chac chan muon xoa khong? (y/n): ";
        char confirm;
        cin >> confirm;
        cin.ignore(10000, '\n');

        if (confirm == 'y' || confirm == 'Y') {
            if (repoHD.removeById(id)) cout << "-> Xoa thanh cong!\n";
        }
        else cout << "Huy thao tac xoa.\n";
    }

    void menu() {
        int choice = -1;
        do {
            xoaManHinh();
            cout << "========== QUAN LY HOA DON CUOC ==========\n";
            cout << "1. Them hoa don cuoc moi\n";
            cout << "2. Xem danh sach\n";
            cout << "3. Tim kiem\n";
            cout << "4. Sap xep theo Tong Tien (Giam dan)\n";
            cout << "5. Cap nhat\n";
            cout << "6. Xoa\n";
            cout << "0. Quay lai menu chinh\n";
            cout << "==========================================\nLua chon cua ban: ";
            if (!(cin >> choice)) {
                cin.clear(); choice = -1;
            }
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
