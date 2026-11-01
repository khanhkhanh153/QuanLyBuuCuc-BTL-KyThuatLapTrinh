
#ifndef KHACHHANGGUI_H
#define KHACHHANGGUI_H

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <stdexcept>
#include <functional>
#include <cstdlib>
#include <cctype>

#include "Entity.h"
#include "Repository.h"

using namespace std;

// --- LOP THUC THE ---
class KhachHangGui : public Entity {
private:
    string hoTen;
    string soDienThoai;
    string diaChi;

    // Kiem tra ho ten
    bool hopLeHoTen(const string& ten) const {
        if (ten.empty()) return false;

        bool coChuCai = false;

        for (unsigned char c : ten) {
            // Cho phep chu cai ASCII
            if (isalpha(c)) {
                coChuCai = true;
            }
            // Cho phep khoang trang
            else if (isspace(c)) {
                continue;
            }
            // Cho phep cac byte UTF-8 cua tieng Viet
            else if (c >= 128) {
                coChuCai = true;
            }
            else {
                return false;
            }
        }

        return coChuCai;
    }

    // Kiem tra so dien thoai chi gom chu so
    bool hopLeSDT(const string& sdt) const {
        if (sdt.empty()) return false;

        for (unsigned char c : sdt) {
            if (!isdigit(c)) {
                return false;
            }
        }

        return true;
    }

public:
    KhachHangGui()
        : Entity(), hoTen(""), soDienThoai(""), diaChi("") {
    }

    string getHoTen() const {
        return hoTen;
    }

    void setHoTen(const string& ten) {
        if (!hopLeHoTen(ten)) {
            throw invalid_argument(
                "Ho ten khong duoc chua so hoac ky tu dac biet!"
            );
        }

        hoTen = ten;
    }

    string getSoDienThoai() const {
        return soDienThoai;
    }

    void setSoDienThoai(const string& sdt) {
        if (!hopLeSDT(sdt)) {
            throw invalid_argument(
                "So dien thoai chi duoc chua cac chu so tu 0 den 9!"
            );
        }

        soDienThoai = sdt;
    }

    string getDiaChi() const {
        return diaChi;
    }

    void setDiaChi(const string& dc) {
        if (dc.empty()) {
            throw invalid_argument("Dia chi khong duoc de trong!");
        }

        diaChi = dc;
    }

    // NHAP THONG TIN
    void nhap() override {
        string temp;

        while (true) {
            try {
                cout << "Nhap ho ten khach hang: ";
                getline(cin, temp);

                setHoTen(temp);
                break;
            }
            catch (const exception& e) {
                cout << "Loi: " << e.what()
                    << " Vui long nhap lai!\n";
            }
        }

        while (true) {
            try {
                cout << "Nhap so dien thoai: ";
                getline(cin, temp);

                setSoDienThoai(temp);
                break;
            }
            catch (const exception& e) {
                cout << "Loi: " << e.what()
                    << " Vui long nhap lai!\n";
            }
        }

        while (true) {
            try {
                cout << "Nhap dia chi: ";
                getline(cin, temp);

                setDiaChi(temp);
                break;
            }
            catch (const exception& e) {
                cout << "Loi: " << e.what()
                    << " Vui long nhap lai!\n";
            }
        }
    }

    // XUAT THONG TIN
    void xuat() const override {
        cout << left << setw(10) << maID
            << "| " << setw(25) << hoTen
            << "| " << setw(15) << soDienThoai
            << "| " << setw(30) << diaChi << endl;
    }

    // GHI FILE
    string toFileRecord() const override {
        stringstream ss;

        ss << maID << "|"
            << hoTen << "|"
            << soDienThoai << "|"
            << diaChi;

        return ss.str();
    }

    // DOC FILE VA KIEM TRA DU LIEU
    void fromFileRecord(const string& record) override {
        stringstream ss(record);
        string item;
        vector<string> tokens;

        while (getline(ss, item, '|')) {
            tokens.push_back(item);
        }

        if (tokens.size() == 4) {
            maID = tokens[0];

            setHoTen(tokens[1]);
            setSoDienThoai(tokens[2]);
            setDiaChi(tokens[3]);
        }
        else {
            throw invalid_argument("Du lieu khach hang trong file khong hop le!");
        }
    }
};


// --- LOP GIAO DIEN (MENU) ---
class KhachHangGuiMenu {
private:
    Repository<KhachHangGui>& repo;
    function<bool(string)> kiemTraRangBuocPhieu;

    void xoaManHinh() const {
        system("cls");
    }

    void tamDung() const {
        cout << "\nNhan Enter de tiep tuc...";
        cin.get();
    }

    void inTieuDeBang() const {
        cout << left << setw(10) << "Ma KH"
            << "| " << setw(25) << "Ho Ten"
            << "| " << setw(15) << "So Dien Thoai"
            << "| " << setw(30) << "Dia Chi" << endl;

        cout << string(85, '-') << endl;
    }

public:
    KhachHangGuiMenu(
        Repository<KhachHangGui>& r,
        function<bool(string)> kiemTra = nullptr
    ) : repo(r), kiemTraRangBuocPhieu(kiemTra) {
    }

    // THEM MOI
    void themMoi() {
        cout << "\n=== THEM MOI KHACH HANG GUI ===\n";

        KhachHangGui kh;
        string id;

        while (true) {
            try {
                cout << "Nhap ma khach hang (VD: KH01): ";
                getline(cin, id);

                kh.setMaID(id);

                if (repo.findById(id) != nullptr) {
                    throw invalid_argument(
                        "Ma khach hang da ton tai!"
                    );
                }

                break;
            }
            catch (const exception& e) {
                cout << "Loi: " << e.what()
                    << " Vui long nhap lai.\n";
            }
        }

        kh.nhap();
        repo.add(kh);

        cout << "-> Them moi thanh cong!\n";
    }

    // XEM DANH SACH
    void xemDanhSach() const {
        auto list = repo.getAll();

        if (list.empty()) {
            cout << "\nDanh sach trong!\n";
            return;
        }

        cout << "\n=========================== DANH SACH KHACH HANG ===========================\n";

        inTieuDeBang();

        for (const auto& item : list) {
            item.xuat();
        }

        cout << "Tong so khach hang: "
            << list.size() << endl;
    }

    // TIM KIEM
    void timKiem() const {
        string id;

        cout << "Nhap ma khach hang can tim: ";
        getline(cin, id);

        KhachHangGui* result = repo.findById(id);

        if (result != nullptr) {
            cout << "\nDa tim thay thong tin:\n";
            inTieuDeBang();
            result->xuat();
        }
        else {
            cout << "Khong tim thay khach hang ma: "
                << id << "\n";
        }
    }

    // SAP XEP
    void sapXep() {
        auto list = repo.getAll();

        if (list.empty()) {
            cout << "\nDanh sach trong!\n";
            return;
        }

        sort(
            list.begin(),
            list.end(),
            [](const KhachHangGui& a, const KhachHangGui& b) {
                return a.getHoTen() < b.getHoTen();
            }
        );

        cout << "-> Da sap xep danh sach theo Ho Ten (A-Z)!\n\n";

        inTieuDeBang();

        for (const auto& item : list) {
            item.xuat();
        }
    }

    // CAP NHAT
    void capNhat() {
        string id;

        cout << "Nhap ma khach hang can cap nhat: ";
        getline(cin, id);

        KhachHangGui* old = repo.findById(id);

        if (old == nullptr) {
            cout << "Khong tim thay ma nay!\n";
            return;
        }

        cout << "\nThong tin hien tai:\n";
        inTieuDeBang();
        old->xuat();

        cout << "\n(De trong neu muon giu nguyen gia tri cu)\n";

        KhachHangGui newData = *old;
        string temp;

        // CAP NHAT HO TEN
        while (true) {
            try {
                cout << "Nhap ho ten moi ["
                    << old->getHoTen() << "]: ";

                getline(cin, temp);

                if (temp.empty()) break;

                newData.setHoTen(temp);
                break;
            }
            catch (const exception& e) {
                cout << "Loi: " << e.what()
                    << " Vui long nhap lai!\n";
            }
        }

        // CAP NHAT SO DIEN THOAI
        while (true) {
            try {
                cout << "Nhap so dien thoai moi ["
                    << old->getSoDienThoai() << "]: ";

                getline(cin, temp);

                if (temp.empty()) break;

                newData.setSoDienThoai(temp);
                break;
            }
            catch (const exception& e) {
                cout << "Loi: " << e.what()
                    << " Vui long nhap lai!\n";
            }
        }

        // CAP NHAT DIA CHI
        while (true) {
            try {
                cout << "Nhap dia chi moi ["
                    << old->getDiaChi() << "]: ";

                getline(cin, temp);

                if (temp.empty()) break;

                newData.setDiaChi(temp);
                break;
            }
            catch (const exception& e) {
                cout << "Loi: " << e.what()
                    << " Vui long nhap lai!\n";
            }
        }

        if (repo.update(id, newData)) {
            cout << "-> Cap nhat thanh cong!\n";
        }
        else {
            cout << "Loi khi cap nhat!\n";
        }
    }

    // XOA
    void xoa() {
        string id;

        cout << "Nhap ma khach hang can xoa: ";
        getline(cin, id);

        KhachHangGui* target = repo.findById(id);

        if (target == nullptr) {
            cout << "Khong tim thay ma nay!\n";
            return;
        }

        cout << "\nThong tin chuan bi xoa:\n";
        inTieuDeBang();
        target->xuat();

        if (
            kiemTraRangBuocPhieu != nullptr &&
            kiemTraRangBuocPhieu(id)
            ) {
            cout << "\n[LOI] KHONG THE XOA!\n";
            cout << "-> Khach hang nay dang co Phieu Khieu Nai hoac Phieu Gui hoat dong.\n";
            return;
        }

        cout << "Ban co chac chan muon xoa khong? (y/n): ";

        char confirm;
        cin >> confirm;
        cin.ignore(10000, '\n');

        if (confirm == 'y' || confirm == 'Y') {
            if (repo.removeById(id)) {
                cout << "-> Xoa thanh cong!\n";
            }
        }
        else {
            cout << "Huy thao tac xoa.\n";
        }
    }

    // MENU
    void menu() {
        int choice = -1;

        do {
            xoaManHinh();

            cout << "========== QUAN LY KHACH HANG ==========\n";
            cout << "1. Them khach hang moi\n";
            cout << "2. Xem danh sach\n";
            cout << "3. Tim kiem\n";
            cout << "4. Sap xep theo Ho Ten\n";
            cout << "5. Cap nhat\n";
            cout << "6. Xoa\n";
            cout << "0. Quay lai menu\n";
            cout << "========================================\nChon: ";

            if (!(cin >> choice)) {
                cin.clear();
                choice = -1;
            }

            cin.ignore(10000, '\n');

            if (choice == 0) break;

            xoaManHinh();

            switch (choice) {
            case 1:
                themMoi();
                break;
            case 2:
                xemDanhSach();
                break;
            case 3:
                timKiem();
                break;
            case 4:
                sapXep();
                break;
            case 5:
                capNhat();
                break;
            case 6:
                xoa();
                break;
            default:
                cout << "Lua chon sai!\n";
            }

            tamDung();

        } while (choice != 0);
    }
};

#endif
