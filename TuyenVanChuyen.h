#ifndef TUYENVANCHUYEN_H
#define TUYENVANCHUYEN_H

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <stdexcept>
#include <functional>
#include <cstdlib>
#include "Entity.h"
#include "Repository.h"
#include "BuuCuc.h"

using namespace std;

// --- LOP THUC THE TUYENVANCHUYEN ---
class TuyenVanChuyen : public Entity {
private:
    string maBuuCucDi;
    string maBuuCucDen;
    double khoangCach;

public:
    TuyenVanChuyen() : Entity(), maBuuCucDi(""), maBuuCucDen(""), khoangCach(0.0) {}

    string getMaBuuCucDi() const { return maBuuCucDi; }
    void setMaBuuCucDi(const string& maBC) {
        if (maBC.empty()) throw invalid_argument("Ma buu cuc di khong duoc de trong!");
        maBuuCucDi = maBC;
    }

    string getMaBuuCucDen() const { return maBuuCucDen; }
    void setMaBuuCucDen(const string& maBC) {
        if (maBC.empty()) throw invalid_argument("Ma buu cuc den khong duoc de trong!");
        maBuuCucDen = maBC;
    }

    double getKhoangCach() const { return khoangCach; }
    void setKhoangCach(double kc) {
        if (kc <= 0) throw invalid_argument("Khoang cach phai lon hon 0!");
        khoangCach = kc;
    }

    void nhap() override {
        while (true) {
            try {
                cout << "Nhap khoang cach (km): ";
                cin >> khoangCach;
                if (cin.fail() || khoangCach <= 0) {
                    cin.clear(); cin.ignore(10000, '\n');
                    throw invalid_argument("Khoang cach phai la so thuc lon hon 0!");
                }
                cin.ignore(10000, '\n');
                break;
            }
            catch (const exception& e) {
                cout << "Loi: " << e.what() << " Vui long nhap lai.\n";
            }
        }
    }

    void xuat() const override {
        cout << left << setw(12) << maID
            << "| " << setw(15) << maBuuCucDi
            << "| " << setw(15) << maBuuCucDen
            << "| " << setw(15) << fixed << setprecision(1) << khoangCach << endl;
    }

    string toFileRecord() const override {
        stringstream ss;
        ss << maID << "|" << maBuuCucDi << "|" << maBuuCucDen << "|" << khoangCach;
        return ss.str();
    }

    void fromFileRecord(const string& record) override {
        stringstream ss(record);
        string item;
        vector<string> tokens;
        while (getline(ss, item, '|')) tokens.push_back(item);
        if (tokens.size() == 4) {
            maID = tokens[0];
            maBuuCucDi = tokens[1];
            maBuuCucDen = tokens[2];
            khoangCach = stod(tokens[3]);
        }
    }
};

// --- LOP GIAO DIEN (MENU TUYENVANCHUYEN) ---
class TuyenVanChuyenMenu {
private:
    Repository<TuyenVanChuyen>& repoTVC;
    Repository<BuuCuc>& repoBC;
    function<bool(string)> kiemTraTrongHoaDon; // Lambda kiem tra rang buoc xoa

    void xoaManHinh() const { system("cls"); }
    void tamDung() const { cout << "\nNhan Enter de tiep tuc..."; cin.get(); }

    void inTieuDeBang() const {
        cout << left << setw(15) << "Ma Tuyen"
            << "| " << setw(25) << "Buu Cuc Di"
            << "| " << setw(25) << "Buu Cuc Den"
            << "| " << setw(15) << "Khoang Cach(km)" << endl;
        cout << string(65, '-') << endl;
    }

public:
    TuyenVanChuyenMenu(Repository<TuyenVanChuyen>& tvc, Repository<BuuCuc>& bc, function<bool(string)> checkHD = nullptr)
        : repoTVC(tvc), repoBC(bc), kiemTraTrongHoaDon(checkHD) {
    }

    void themMoi() {
        cout << "\n=== THEM MOI TUYEN VAN CHUYEN ===\n";
        TuyenVanChuyen tvc;
        string temp;

        while (true) {
            try {
                cout << "Nhap ma tuyen van chuyen (VD: TVC01): ";
                getline(cin, temp);
                tvc.setMaID(temp);
                if (repoTVC.findById(temp) != nullptr) throw invalid_argument("Ma tuyen van chuyen da ton tai!");
                break;
            }
            catch (const exception& e) {
                cout << "Loi: " << e.what() << " Vui long nhap lai.\n";
            }
        }

        while (true) {
            try {
                cout << "Nhap ma Buu cuc di (VD: BC01): ";
                getline(cin, temp);
                if (temp.empty()) throw invalid_argument("Khong duoc de trong!");
                if (repoBC.findById(temp) == nullptr) throw invalid_argument("Buu cuc di nay chua ton tai!");
                tvc.setMaBuuCucDi(temp);
                break;
            }
            catch (const exception& e) {
                cout << "Loi: " << e.what() << "\n";
            }
        }

        while (true) {
            try {
                cout << "Nhap ma Buu cuc den (VD: BC02): ";
                getline(cin, temp);
                if (temp.empty()) throw invalid_argument("Khong duoc de trong!");
                if (repoBC.findById(temp) == nullptr) throw invalid_argument("Buu cuc den nay chua ton tai!");
                if (temp == tvc.getMaBuuCucDi()) throw invalid_argument("Buu cuc den phai khac buu cuc di!");
                tvc.setMaBuuCucDen(temp);
                break;
            }
            catch (const exception& e) {
                cout << "Loi: " << e.what() << "\n";
            }
        }

        tvc.nhap();
        repoTVC.add(tvc);
        cout << "-> Them tuyen van chuyen thanh cong!\n";
    }

    void xemDanhSach() const {
        auto list = repoTVC.getAll();
        if (list.empty()) {
            cout << "\nDanh sach tuyen van chuyen trong!\n";
            return;
        }
        cout << "\n==================== DANH SACH TUYEN VAN CHUYEN ====================\n";
        inTieuDeBang();
        for (const auto& item : list) item.xuat();
        cout << "Tong so tuyen van chuyen: " << list.size() << endl;
    }

    void timKiem() const {
        string id;
        cout << "Nhap ma tuyen van chuyen can tim: ";
        getline(cin, id);
        TuyenVanChuyen* result = repoTVC.findById(id);
        if (result != nullptr) {
            cout << "\nDa tim thay thong tin:\n";
            inTieuDeBang();
            result->xuat();
        }
        else cout << "Khong tim thay tuyen van chuyen ma: " << id << "\n";
    }

    void sapXep() {
        auto list = repoTVC.getAll();
        if (list.empty()) {
            cout << "\nDanh sach trong!\n";
            return;
        }

        sort(list.begin(), list.end(), [](const TuyenVanChuyen& a, const TuyenVanChuyen& b) {
            return a.getKhoangCach() < b.getKhoangCach();
            });

        cout << "-> Da sap xep danh sach theo Khoang Cach (Tang dan)!\n";
        cout << "\n==================== KET QUA SAP XEP ====================\n";
        inTieuDeBang();
        for (const auto& item : list) item.xuat();
    }

    void capNhat() {
        string id;
        cout << "Nhap ma tuyen can cap nhat: ";
        getline(cin, id);
        TuyenVanChuyen* old = repoTVC.findById(id);
        if (old == nullptr) {
            cout << "Khong tim thay ma nay!\n";
            return;
        }

        cout << "\nThong tin hien tai:\n";
        inTieuDeBang();
        old->xuat();
        cout << "\n(Luu y: De trong hoac nhap -1 neu muon giu nguyen gia tri cu)\n";

        TuyenVanChuyen newData = *old;
        string temp;

        cout << "Nhap ma Buu cuc di moi [" << old->getMaBuuCucDi() << "]: ";
        getline(cin, temp);
        if (!temp.empty()) {
            if (repoBC.findById(temp) == nullptr) cout << "Loi: Buu cuc khong ton tai! Giu nguyen ma cu.\n";
            else newData.setMaBuuCucDi(temp);
        }

        cout << "Nhap ma Buu cuc den moi [" << old->getMaBuuCucDen() << "]: ";
        getline(cin, temp);
        if (!temp.empty()) {
            if (repoBC.findById(temp) == nullptr) cout << "Loi: Buu cuc khong ton tai! Giu nguyen ma cu.\n";
            else if (temp == newData.getMaBuuCucDi()) cout << "Loi: Buu cuc den phai khac buu cuc di! Giu nguyen ma cu.\n";
            else newData.setMaBuuCucDen(temp);
        }

        try {
            cout << "Nhap khoang cach moi [" << fixed << setprecision(1) << old->getKhoangCach() << "]: ";
            getline(cin, temp);
            if (!temp.empty() && stod(temp) != -1) newData.setKhoangCach(stod(temp));
        }
        catch (...) {}

        if (repoTVC.update(id, newData)) cout << "-> Cap nhat thanh cong!\n";
        else cout << "Loi khi cap nhat!\n";
    }

    void xoa() {
        string id;
        cout << "Nhap ma tuyen van chuyen can xoa: ";
        getline(cin, id);
        TuyenVanChuyen* target = repoTVC.findById(id);
        if (target == nullptr) {
            cout << "Khong tim thay ma nay!\n";
            return;
        }

        // KIEM TRA RANG BUOC KHONG CHO XOA NEU CO TRONG HOA DON
        if (kiemTraTrongHoaDon && kiemTraTrongHoaDon(id)) {
            cout << "-> KHONG THE XOA! Tuyen van chuyen ma [" << id << "] dang duoc dung trong Hoa Don Cuoc Phi.\n";
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
            if (repoTVC.removeById(id)) cout << "-> Xoa thanh cong!\n";
        }
        else cout << "Huy thao tac xoa.\n";
    }

    void menu() {
        int choice = -1;
        do {
            xoaManHinh();
            cout << "========== QUAN LY TUYEN VAN CHUYEN ==========\n";
            cout << "1. Them tuyen van chuyen moi\n";
            cout << "2. Xem danh sach\n";
            cout << "3. Tim kiem\n";
            cout << "4. Sap xep theo Khoang Cach (Tang dan)\n";
            cout << "5. Cap nhat\n";
            cout << "6. Xoa\n";
            cout << "0. Quay lai menu chinh\n";
            cout << "==============================================\nLua chon cua ban: ";
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
