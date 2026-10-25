//MAI DUC ANH
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
#include <limits>
#include <cstdlib>
#include "Entity.h"
#include "Repository.h"

using namespace std;

// ============================================================
// LOP THUC THE KHACH HANG GUI
// ============================================================
class KhachHangGui : public Entity {
private:
    string hoTen;
    string soDienThoai;
    string diaChi;
    string email;

public:
    KhachHangGui()
        : Entity(), hoTen(""), soDienThoai(""), diaChi(""), email("") {}

    KhachHangGui(const string& ma, const string& ten, const string& sdt,
                 const string& dc, const string& mail)
        : Entity(ma), hoTen(ten), soDienThoai(sdt), diaChi(dc), email(mail) {}

    string getMaKhachHang() const { return maID; }
    string getHoTen() const { return hoTen; }
    string getSoDienThoai() const { return soDienThoai; }
    string getDiaChi() const { return diaChi; }
    string getEmail() const { return email; }

    void setHoTen(const string& ten) {
        if (ten.empty()) throw invalid_argument("Ho ten khong duoc de trong!");
        hoTen = ten;
    }

    void setSoDienThoai(const string& sdt) {
        if (sdt.empty()) throw invalid_argument("So dien thoai khong duoc de trong!");
        soDienThoai = sdt;
    }

    void setDiaChi(const string& dc) {
        if (dc.empty()) throw invalid_argument("Dia chi khong duoc de trong!");
        diaChi = dc;
    }

    void setEmail(const string& mail) {
        if (mail.empty()) throw invalid_argument("Email khong duoc de trong!");
        email = mail;
    }

    void nhap() override {
        string temp;
        while (true) {
            try {
                cout << "Nhap ma khach hang (VD: KH001): ";
                getline(cin >> ws, temp);
                setMaID(temp);
                break;
            } catch (const exception& e) {
                cout << "Loi: " << e.what() << " Vui long nhap lai.\n";
            }
        }

        while (true) {
            try {
                cout << "Nhap ho ten: ";
                getline(cin, temp);
                setHoTen(temp);
                break;
            } catch (const exception& e) {
                cout << "Loi: " << e.what() << " Vui long nhap lai.\n";
            }
        }

        while (true) {
            try {
                cout << "Nhap so dien thoai: ";
                getline(cin, temp);
                setSoDienThoai(temp);
                break;
            } catch (const exception& e) {
                cout << "Loi: " << e.what() << " Vui long nhap lai.\n";
            }
        }

        while (true) {
            try {
                cout << "Nhap dia chi: ";
                getline(cin, temp);
                setDiaChi(temp);
                break;
            } catch (const exception& e) {
                cout << "Loi: " << e.what() << " Vui long nhap lai.\n";
            }
        }

        while (true) {
            try {
                cout << "Nhap email: ";
                getline(cin, temp);
                setEmail(temp);
                break;
            } catch (const exception& e) {
                cout << "Loi: " << e.what() << " Vui long nhap lai.\n";
            }
        }
    }

    void xuat() const override {
        cout << left << setw(12) << maID
             << "| " << setw(25) << hoTen
             << "| " << setw(16) << soDienThoai
             << "| " << setw(28) << diaChi
             << "| " << email << '\n';
    }

    string toFileRecord() const override {
        stringstream ss;
        ss << maID << '|' << hoTen << '|' << soDienThoai << '|'
           << diaChi << '|' << email;
        return ss.str();
    }

    void fromFileRecord(const string& record) override {
        stringstream ss(record);
        vector<string> tokens;
        string item;
        while (getline(ss, item, '|')) tokens.push_back(item);
        if (tokens.size() != 5 || tokens[0].empty()) {
            throw invalid_argument("Du lieu khach hang khong dung dinh dang!");
        }
        maID = tokens[0];
        hoTen = tokens[1];
        soDienThoai = tokens[2];
        diaChi = tokens[3];
        email = tokens[4];
    }
};

// ============================================================
// LOP GIAO DIEN VA QUAN LY MENU KHACH HANG GUI
// ============================================================
class KhachHangGuiMenu {
private:
    Repository<KhachHangGui>& repoKH;
    function<bool(string)> kiemTraDangDuocThamChieu;

    void xoaManHinh() const {
#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif
    }

    void tamDung() const {
        cout << "\nNhan Enter de tiep tuc...";
        cin.get();
    }

    void inTieuDeBang() const {
        cout << left << setw(12) << "Ma KH"
             << "| " << setw(25) << "Ho Ten"
             << "| " << setw(16) << "So Dien Thoai"
             << "| " << setw(28) << "Dia Chi"
             << "| Email\n";
        cout << string(105, '-') << '\n';
    }

public:
    explicit KhachHangGuiMenu(
        Repository<KhachHangGui>& repository,
        function<bool(string)> checkThamChieu = nullptr)
        : repoKH(repository), kiemTraDangDuocThamChieu(checkThamChieu) {}

    void themMoi() {
        cout << "\n=== THEM MOI KHACH HANG GUI ===\n";
        KhachHangGui kh;
        kh.nhap();
        try {
            repoKH.add(kh);
            cout << "-> Them khach hang thanh cong!\n";
        } catch (const exception& e) {
            cout << "-> Khong the them khach hang: " << e.what() << '\n';
        }
    }

    void xemDanhSach() const {
        const auto danhSach = repoKH.getAll();
        if (danhSach.empty()) {
            cout << "\nDanh sach khach hang gui trong!\n";
            return;
        }
        cout << "\n========================= DANH SACH KHACH HANG GUI =========================\n";
        inTieuDeBang();
        for (const auto& kh : danhSach) kh.xuat();
        cout << "Tong so khach hang: " << danhSach.size() << '\n';
    }

    void timKiem() {
        string ma;
        cout << "Nhap ma khach hang can tim: ";
        getline(cin >> ws, ma);
        KhachHangGui* kh = repoKH.findById(ma);
        if (kh) {
            cout << "\nDa tim thay khach hang:\n";
            inTieuDeBang();
            kh->xuat();
        } else {
            cout << "Khong tim thay khach hang co ma: " << ma << '\n';
        }
    }

    void capNhat() {
        string ma;
        cout << "Nhap ma khach hang can cap nhat: ";
        getline(cin >> ws, ma);
        KhachHangGui* cu = repoKH.findById(ma);
        if (cu == nullptr) {
            cout << "Khong tim thay ma khach hang nay!\n";
            return;
        }

        cout << "\nThong tin hien tai:\n";
        inTieuDeBang();
        cu->xuat();
        cout << "Nhap thong tin moi cho khach hang (ma KH giu nguyen: " << ma << ")\n";

        KhachHangGui moi;
        moi.nhap();
        moi.setMaID(ma);
        if (repoKH.update(ma, moi)) cout << "-> Cap nhat khach hang thanh cong!\n";
        else cout << "-> Cap nhat that bai!\n";
    }

    void xoa() {
        string ma;
        cout << "Nhap ma khach hang can xoa: ";
        getline(cin >> ws, ma);
        KhachHangGui* kh = repoKH.findById(ma);
        if (kh == nullptr) {
            cout << "Khong tim thay ma khach hang nay!\n";
            return;
        }

        // Rang buoc tham chieu duoc truyen tu Application.
        // Callback can kiem tra ca Phieu Gui va Phieu Khieu Nai.
        if (kiemTraDangDuocThamChieu && kiemTraDangDuocThamChieu(ma)) {
            cout << "-> KHONG THE XOA! Khach hang ma [" << ma
                 << "] dang duoc tham chieu trong Phieu Gui hoac Phieu Khieu Nai.\n";
            return;
        }

        cout << "\nThong tin chuan bi xoa:\n";
        inTieuDeBang();
        kh->xuat();
        cout << "Ban co chac chan muon xoa khong? (y/n): ";
        char xacNhan;
        cin >> xacNhan;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (xacNhan == 'y' || xacNhan == 'Y') {
            if (repoKH.removeById(ma)) cout << "-> Xoa khach hang thanh cong!\n";
            else cout << "-> Xoa khach hang that bai!\n";
        } else {
            cout << "Huy thao tac xoa.\n";
        }
    }

    void menu() {
        int choice = -1;
        do {
            cout << "\n========== QUAN LY KHACH HANG GUI ==========\n"
                 << "1. Them khach hang moi\n"
                 << "2. Xem danh sach khach hang\n"
                 << "3. Tim kiem khach hang theo ma\n"
                 << "4. Cap nhat thong tin khach hang\n"
                 << "5. Xoa khach hang\n"
                 << "0. Quay lai menu chinh\n"
                 << "============================================\n"
                 << "Lua chon cua ban: ";

            if (!(cin >> choice)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Lua chon khong hop le!\n";
                continue;
            }
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            if (choice == 0) break;

            switch (choice) {
                case 1: themMoi(); break;
                case 2: xemDanhSach(); break;
                case 3: timKiem(); break;
                case 4: capNhat(); break;
                case 5: xoa(); break;
                default: cout << "Vui long chon tu 0 den 5!\n";
            }
            tamDung();
        } while (choice != 0);
    }
};

#endif
