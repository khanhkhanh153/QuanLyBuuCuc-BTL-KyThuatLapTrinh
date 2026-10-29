
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
#include <cctype>

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
        : Entity(), hoTen(""), soDienThoai(""),
          diaChi(""), email("") {}

    KhachHangGui(const string& ma, const string& ten,
                 const string& sdt, const string& dc,
                 const string& mail)
        : Entity(ma), hoTen(""), soDienThoai(""),
          diaChi(""), email("") {
        setHoTen(ten);
        setSoDienThoai(sdt);
        setDiaChi(dc);
        setEmail(mail);
    }

    // ================= GETTER =================
    string getMaKhachHang() const {
        return maID;
    }

    string getHoTen() const {
        return hoTen;
    }

    string getSoDienThoai() const {
        return soDienThoai;
    }

    string getDiaChi() const {
        return diaChi;
    }

    string getEmail() const {
        return email;
    }

    // ================= SETTER =================
    void setHoTen(const string& ten) {
        if (ten.empty()) {
            throw invalid_argument(
                "Ho ten khong duoc de trong!"
            );
        }

        // Kiem tra ten:
        // - Cho phep chu cai ASCII.
        // - Cho phep khoang trang.
        // - Cho phep byte UTF-8 de ho tro tieng Viet co dau.
        // - Khong cho phep chu so va ky tu dac biet ASCII.

        bool coChuCai = false;

        for (unsigned char c : ten) {
            if (c < 128) {
                if (isalpha(c)) {
                    coChuCai = true;
                }
                else if (isspace(c)) {
                    // Cho phep khoang trang.
                }
                else {
                    throw invalid_argument(
                        "Ho ten khong duoc chua so "
                        "hoac ky tu dac biet!"
                    );
                }
            }
            else {
                // Chap nhan byte UTF-8 cua chu co dau.
                coChuCai = true;
            }
        }

        if (!coChuCai) {
            throw invalid_argument(
                "Ho ten phai co chu cai!"
            );
        }

        hoTen = ten;
    }

    void setSoDienThoai(const string& sdt) {
        if (sdt.empty()) {
            throw invalid_argument(
                "So dien thoai khong duoc de trong!"
            );
        }

        // Chi cho phep cac chu so tu 0 den 9.
        for (unsigned char c : sdt) {
            if (!isdigit(c)) {
                throw invalid_argument(
                    "So dien thoai chi duoc chua "
                    "chu so tu 0 den 9!"
                );
            }
        }

        soDienThoai = sdt;
    }

    void setDiaChi(const string& dc) {
        if (dc.empty()) {
            throw invalid_argument(
                "Dia chi khong duoc de trong!"
            );
        }

        diaChi = dc;
    }

    void setEmail(const string& mail) {
        if (mail.empty()) {
            throw invalid_argument(
                "Email khong duoc de trong!"
            );
        }

        email = mail;
    }

    // ================= NHAP THONG TIN =================
    void nhap() override {
        string temp;

        // Nhap ma khach hang
        while (true) {
            try {
                cout << "Nhap ma khach hang (VD: KH001): ";
                getline(cin >> ws, temp);

                setMaID(temp);
                break;
            }
            catch (const exception& e) {
                cout << "Loi: " << e.what()
                     << " Vui long nhap lai.\n";
            }
        }

        // Nhap ho ten
        while (true) {
            try {
                cout << "Nhap ho ten: ";
                getline(cin, temp);

                setHoTen(temp);
                break;
            }
            catch (const exception& e) {
                cout << "Loi: " << e.what()
                     << " Vui long nhap lai.\n";
            }
        }

        // Nhap so dien thoai
        while (true) {
            try {
                cout << "Nhap so dien thoai: ";
                getline(cin, temp);

                setSoDienThoai(temp);
                break;
            }
            catch (const exception& e) {
                cout << "Loi: " << e.what()
                     << " Vui long nhap lai.\n";
            }
        }

        // Nhap dia chi
        while (true) {
            try {
                cout << "Nhap dia chi: ";
                getline(cin, temp);

                setDiaChi(temp);
                break;
            }
            catch (const exception& e) {
                cout << "Loi: " << e.what()
                     << " Vui long nhap lai.\n";
            }
        }

        // Nhap email
        while (true) {
            try {
                cout << "Nhap email: ";
                getline(cin, temp);

                setEmail(temp);
                break;
            }
            catch (const exception& e) {
                cout << "Loi: " << e.what()
                     << " Vui long nhap lai.\n";
            }
        }
    }

    // ================= XUAT THONG TIN =================
    void xuat() const override {
        cout << left << setw(12) << maID
             << "| " << setw(25) << hoTen
             << "| " << setw(16) << soDienThoai
             << "| " << setw(28) << diaChi
             << "| " << email << '\n';
    }

    // ================= GHI FILE =================
    string toFileRecord() const override {
        stringstream ss;

        ss << maID << '|'
           << hoTen << '|'
           << soDienThoai << '|'
           << diaChi << '|'
           << email;

        return ss.str();
    }

    // ================= DOC FILE =================
    void fromFileRecord(const string& record) override {
        stringstream ss(record);
        vector<string> tokens;
        string item;

        while (getline(ss, item, '|')) {
            tokens.push_back(item);
        }

        if (tokens.size() != 5 || tokens[0].empty()) {
            throw invalid_argument(
                "Du lieu khach hang khong dung dinh dang!"
            );
        }

        // Gan thong tin thong qua setter de kiem tra du lieu.
        setMaID(tokens[0]);
        setHoTen(tokens[1]);
        setSoDienThoai(tokens[2]);
        setDiaChi(tokens[3]);
        setEmail(tokens[4]);
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
        function<bool(string)> checkThamChieu = nullptr
    )
        : repoKH(repository),
          kiemTraDangDuocThamChieu(checkThamChieu) {}

    // ================= THEM KHACH HANG =================
    void themMoi() {
        cout << "\n=== THEM MOI KHACH HANG GUI ===\n";

        KhachHangGui kh;
        kh.nhap();

        try {
            repoKH.add(kh);
            cout << "-> Them khach hang thanh cong!\n";
        }
        catch (const exception& e) {
            cout << "-> Khong the them khach hang: "
                 << e.what() << '\n';
        }
    }

    // ================= XEM DANH SACH =================
    void xemDanhSach() const {
        const auto danhSach = repoKH.getAll();

        if (danhSach.empty()) {
            cout << "\nDanh sach khach hang gui trong!\n";
            return;
        }

        cout << "\n========================= DANH SACH KHACH HANG GUI =========================\n";

        inTieuDeBang();

        for (const auto& kh : danhSach) {
            kh.xuat();
        }

        cout << "Tong so khach hang: "
             << danhSach.size() << '\n';
    }

    // ================= TIM KIEM =================
    void timKiem() {
        string ma;

        cout << "Nhap ma khach hang can tim: ";
        getline(cin >> ws, ma);

        KhachHangGui* kh = repoKH.findById(ma);

        if (kh != nullptr) {
            cout << "\nDa tim thay khach hang:\n";
            inTieuDeBang();
            kh->xuat();
        }
        else {
            cout << "Khong tim thay khach hang co ma: "
                 << ma << '\n';
        }
    }

    // ================= CAP NHAT =================
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

        cout << "Nhap thong tin moi cho khach hang "
             << "(ma KH giu nguyen: " << ma << ")\n";

        KhachHangGui moi;
        moi.nhap();

        // Giu nguyen ma khach hang cu.
        moi.setMaID(ma);

        if (repoKH.update(ma, moi)) {
            cout << "-> Cap nhat khach hang thanh cong!\n";
        }
        else {
            cout << "-> Cap nhat that bai!\n";
        }
    }

    // ================= XOA KHACH HANG =================
    void xoa() {
        string ma;

        cout << "Nhap ma khach hang can xoa: ";
        getline(cin >> ws, ma);

        KhachHangGui* kh = repoKH.findById(ma);

        if (kh == nullptr) {
            cout << "Khong tim thay ma khach hang nay!\n";
            return;
        }

        // Kiem tra khach hang co dang duoc tham chieu hay khong.
        // Callback can kiem tra ca Phieu Gui va Phieu Khieu Nai.
        if (kiemTraDangDuocThamChieu &&
            kiemTraDangDuocThamChieu(ma)) {

            cout << "-> KHONG THE XOA! Khach hang ma ["
                 << ma
                 << "] dang duoc tham chieu trong "
                 << "Phieu Gui hoac Phieu Khieu Nai.\n";
            return;
        }

        cout << "\nThong tin chuan bi xoa:\n";
        inTieuDeBang();
        kh->xuat();

        cout << "Ban co chac chan muon xoa khong? (y/n): ";

        char xacNhan;
        cin >> xacNhan;

        cin.ignore(
            numeric_limits<streamsize>::max(), '\n'
        );

        if (xacNhan == 'y' || xacNhan == 'Y') {
            if (repoKH.removeById(ma)) {
                cout << "-> Xoa khach hang thanh cong!\n";
            }
            else {
                cout << "-> Xoa khach hang that bai!\n";
            }
        }
        else {
            cout << "Huy thao tac xoa.\n";
        }
    }

    // ================= MENU KHACH HANG =================
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

                cin.ignore(
                    numeric_limits<streamsize>::max(), '\n'
                );

                cout << "Lua chon khong hop le!\n";
                continue;
            }

            cin.ignore(
                numeric_limits<streamsize>::max(), '\n'
            );

            if (choice == 0) {
                break;
            }

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
                    capNhat();
                    break;

                case 5:
                    xoa();
                    break;

                default:
                    cout << "Vui long chon tu 0 den 5!\n";
            }

            tamDung();

        } while (choice != 0);
    }
};

#endif
