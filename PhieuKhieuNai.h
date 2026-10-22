#ifndef PHIEUKHIEUNAI_H
#define PHIEUKHIEUNAI_H

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
#include "KhachHangGui.h" // De kiem tra khoa ngoai

using namespace std;

// --- LOP THUC THE ---
class PhieuKhieuNai : public Entity {
private:
    string noiDung;
    string trangThai; // "Cho xu ly", "Dang xu ly", "Da hoan thanh"
    string maKhachHang; // Khoa ngoai tham chieu

public:
    PhieuKhieuNai() : Entity(), noiDung(""), trangThai("Cho xu ly"), maKhachHang("") {}

    string getNoiDung() const { return noiDung; }
    void setNoiDung(const string& nd) {
        if (nd.empty()) throw invalid_argument("Noi dung khong duoc de trong!");
        noiDung = nd;
    }

    string getTrangThai() const { return trangThai; }
    void setTrangThai(const string& tt) {
        if (tt.empty()) throw invalid_argument("Trang thai khong duoc de trong!");
        trangThai = tt;
    }

    string getMaKhachHang() const { return maKhachHang; }
    void setMaKhachHang(const string& maKH) {
        if (maKH.empty()) throw invalid_argument("Ma khach hang khong duoc de trong!");
        maKhachHang = maKH;
    }

    void nhap() override {
        string temp;
        while (true) {
            try {
                cout << "Nhap noi dung khieu nai: "; getline(cin, temp);
                setNoiDung(temp); break;
            }
            catch (const exception& e) { cout << "Loi: " << e.what() << " Vui long nhap lai.\n"; }
        }
        while (true) {
            try {
                cout << "Nhap trang thai (VD: Cho xu ly/Da xu ly): "; getline(cin, temp);
                setTrangThai(temp); break;
            }
            catch (const exception& e) { cout << "Loi: " << e.what() << " Vui long nhap lai.\n"; }
        }
    }

    void xuat() const override {
        cout << left << setw(10) << maID << "| " << setw(15) << maKhachHang
            << "| " << setw(40) << noiDung << "| " << setw(15) << trangThai << endl;
    }

    string toFileRecord() const override {
        stringstream ss;
        ss << maID << "|" << maKhachHang << "|" << noiDung << "|" << trangThai;
        return ss.str();
    }

    void fromFileRecord(const string& record) override {
        stringstream ss(record);
        string item; vector<string> tokens;
        while (getline(ss, item, '|')) tokens.push_back(item);
        if (tokens.size() == 4) {
            maID = tokens[0]; maKhachHang = tokens[1];
            noiDung = tokens[2]; trangThai = tokens[3];
        }
    }
};

// --- LOP GIAO DIEN (MENU) ---
class PhieuKhieuNaiMenu {
private:
    Repository<PhieuKhieuNai>& repoPKN;
    Repository<KhachHangGui>& repoKH;

    void xoaManHinh() const { system("cls"); }
    void tamDung() const { cout << "\nNhan Enter de tiep tuc..."; cin.get(); }
    void inTieuDeBang() const {
        cout << left << setw(10) << "Ma KN" << "| " << setw(15) << "Ma Khach Hang"
            << "| " << setw(40) << "Noi Dung" << "| " << setw(15) << "Trang Thai" << endl;
        cout << string(80, '-') << endl;
    }

public:
    PhieuKhieuNaiMenu(Repository<PhieuKhieuNai>& pkn, Repository<KhachHangGui>& kh)
        : repoPKN(pkn), repoKH(kh) {
    }

    void themMoi() {
        cout << "\n=== THEM MOI PHIEU KHIEU NAI ===\n";
        PhieuKhieuNai pkn; string temp;

        while (true) {
            try {
                cout << "Nhap ma phieu (VD: KN01): "; getline(cin, temp);
                pkn.setMaID(temp);
                if (repoPKN.findById(temp) != nullptr) throw invalid_argument("Ma phieu da ton tai!");
                break;
            }
            catch (const exception& e) { cout << "Loi: " << e.what() << "\n"; }
        }

        // KIEM TRA KHOA NGOAI
        while (true) {
            try {
                cout << "Nhap ma Khach hang (VD: KH01): "; getline(cin, temp);
                if (temp.empty()) throw invalid_argument("Khong duoc de trong!");
                if (repoKH.findById(temp) == nullptr) {
                    throw invalid_argument("Khach hang nay chua ton tai he thong! Vui long kiem tra lai.");
                }
                pkn.setMaKhachHang(temp);
                break;
            }
            catch (const exception& e) { cout << "Loi: " << e.what() << "\n"; }
        }

        pkn.nhap(); repoPKN.add(pkn);
        cout << "-> Them moi thanh cong!\n";
    }

    void xemDanhSach() const {
        auto list = repoPKN.getAll();
        if (list.empty()) { cout << "\nDanh sach trong!\n"; return; }
        cout << "\n========================= DANH SACH KHIEU NAI =========================\n";
        inTieuDeBang();
        for (const auto& item : list) item.xuat();
        cout << "Tong so khieu nai: " << list.size() << endl;
    }

    void timKiem() const {
        string id; cout << "Nhap ma phieu khieu nai: "; getline(cin, id);
        PhieuKhieuNai* result = repoPKN.findById(id);
        if (result != nullptr) {
            cout << "\nDa tim thay thong tin:\n"; inTieuDeBang(); result->xuat();
        }
        else cout << "Khong tim thay!\n";
    }

    void sapXep() {
        auto list = repoPKN.getAll();
        if (list.empty()) { cout << "\nDanh sach trong!\n"; return; }

        sort(list.begin(), list.end(), [](const PhieuKhieuNai& a, const PhieuKhieuNai& b) {
            return a.getTrangThai() > b.getTrangThai(); // Sap xep de gom nhom trang thai
            });
        cout << "-> Da sap xep danh sach theo Trang Thai!\n\n";
        inTieuDeBang();
        for (const auto& item : list) item.xuat();
    }

    void capNhat() {
        string id; cout << "Nhap ma phieu can cap nhat: "; getline(cin, id);
        PhieuKhieuNai* old = repoPKN.findById(id);
        if (old == nullptr) { cout << "Khong tim thay ma nay!\n"; return; }

        cout << "\nThong tin hien tai:\n"; inTieuDeBang(); old->xuat();
        cout << "\n(Luu y: De trong neu muon giu nguyen gia tri cu)\n";

        PhieuKhieuNai newData = *old; string temp;

        cout << "Nhap ma khach hang moi [" << old->getMaKhachHang() << "]: "; getline(cin, temp);
        if (!temp.empty()) {
            if (repoKH.findById(temp) == nullptr) cout << "Loi: Khach hang khong ton tai! Giu nguyen ma cu.\n";
            else newData.setMaKhachHang(temp);
        }

        cout << "Nhap noi dung moi [" << old->getNoiDung() << "]: "; getline(cin, temp);
        if (!temp.empty()) newData.setNoiDung(temp);

        cout << "Nhap trang thai moi [" << old->getTrangThai() << "]: "; getline(cin, temp);
        if (!temp.empty()) newData.setTrangThai(temp);

        if (repoPKN.update(id, newData)) cout << "-> Cap nhat thanh cong!\n";
        else cout << "Loi khi cap nhat!\n";
    }

    void xoa() {
        string id; cout << "Nhap ma phieu can xoa: "; getline(cin, id);
        if (repoPKN.findById(id) == nullptr) { cout << "Khong tim thay!\n"; return; }

        cout << "Ban co chac chan muon xoa khong? (y/n): ";
        char confirm; cin >> confirm; cin.ignore(10000, '\n');
        if (confirm == 'y' || confirm == 'Y') {
            if (repoPKN.removeById(id)) cout << "-> Xoa thanh cong!\n";
        }
        else cout << "Huy thao tac xoa.\n";
    }

    void menu() {
        int choice = -1;
        do {
            xoaManHinh();
            cout << "========== QUAN LY PHIEU KHIEU NAI ==========\n";
            cout << "1. Them khieu nai\n2. Xem danh sach\n3. Tim kiem\n4. Sap xep theo Trang thai\n5. Cap nhat\n6. Xoa\n0. Quay lai\n";
            cout << "=============================================\nChon: ";
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
            default: cout << "Lua chon sai!\n";
            }
            tamDung();
        } while (choice != 0);
    }
};

#endif