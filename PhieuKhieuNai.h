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
#include <functional>
#include "Entity.h"
#include "Repository.h"
#include "BuuCuc.h"
#include "KhachHangGui.h" // Can co class KhachHangGui ke thua Entity

using namespace std;

// ============================================================
// LOP THUC THE: PHIEU KHIẾU NAI
// MaKHGui va maBuuCuc la khoa tham chieu den du lieu da ton tai.
// ============================================================
class PhieuKhieuNai : public Entity {
private:
    string maKhachHangGui;
    string maBuuCuc;
    string noiDung;
    string trangThai;

public:
    PhieuKhieuNai()
        : Entity(), maKhachHangGui(""), maBuuCuc(""),
          noiDung(""), trangThai("Chua xu ly") {}

    string getMaKhachHangGui() const { return maKhachHangGui; }
    void setMaKhachHangGui(const string& maKH) {
        if (maKH.empty()) throw invalid_argument("Ma khach hang gui khong duoc de trong!");
        maKhachHangGui = maKH;
    }

    string getMaBuuCuc() const { return maBuuCuc; }
    void setMaBuuCuc(const string& maBC) {
        if (maBC.empty()) throw invalid_argument("Ma buu cuc khong duoc de trong!");
        maBuuCuc = maBC;
    }

    string getNoiDung() const { return noiDung; }
    void setNoiDung(const string& nd) {
        if (nd.empty()) throw invalid_argument("Noi dung khieu nai khong duoc de trong!");
        noiDung = nd;
    }

    string getTrangThai() const { return trangThai; }
    void setTrangThai(const string& tt) {
        if (tt.empty()) throw invalid_argument("Trang thai khong duoc de trong!");
        trangThai = tt;
    }

    // Nhap cac truong noi dung; hai ma tham chieu duoc menu kiem tra.
    void nhap() override {
        string temp;
        while (true) {
            try {
                cout << "Nhap noi dung khieu nai: ";
                getline(cin, temp);
                setNoiDung(temp);
                break;
            } catch (const exception& e) {
                cout << "Loi: " << e.what() << " Vui long nhap lai.\n";
            }
        }

        cout << "Nhap trang thai [mac dinh: Chua xu ly]: ";
        getline(cin, temp);
        if (!temp.empty()) setTrangThai(temp);
    }

    void xuat() const override {
        cout << left << setw(12) << maID
             << "| " << setw(15) << maKhachHangGui
             << "| " << setw(12) << maBuuCuc
             << "| " << setw(35) << noiDung
             << "| " << setw(18) << trangThai << '\n';
    }

    string toFileRecord() const override {
        stringstream ss;
        ss << maID << "|" << maKhachHangGui << "|" << maBuuCuc
           << "|" << noiDung << "|" << trangThai;
        return ss.str();
    }

    void fromFileRecord(const string& record) override {
        stringstream ss(record);
        string item;
        vector<string> tokens;
        while (getline(ss, item, '|')) tokens.push_back(item);
        if (tokens.size() == 5) {
            maID = tokens[0];
            maKhachHangGui = tokens[1];
            maBuuCuc = tokens[2];
            noiDung = tokens[3];
            trangThai = tokens[4];
        }
    }
};

// ============================================================
// MENU: Kiem tra ma khach hang va ma buu cuc truoc khi ghi file
// ============================================================
class PhieuKhieuNaiMenu {
private:
    Repository<PhieuKhieuNai>& repoPKN;
    Repository<BuuCuc>& repoBC;
    Repository<KhachHangGui>& repoKH;

    void xoaManHinh() const { system("cls"); }
    void tamDung() const {
        cout << "\nNhan Enter de tiep tuc...";
        cin.get();
    }
    void inTieuDeBang() const {
        cout << left << setw(12) << "Ma PKN"
             << "| " << setw(15) << "Ma KH Gui"
             << "| " << setw(12) << "Ma BC"
             << "| " << setw(35) << "Noi dung"
             << "| " << setw(18) << "Trang thai" << '\n';
        cout << string(102, '-') << '\n';
    }

    string nhapMaKhachHangHopLe(const string& maMacDinh = "") {
        string ma;
        while (true) {
            cout << "Nhap ma khach hang gui";
            if (!maMacDinh.empty()) cout << " [" << maMacDinh << "]";
            cout << ": ";
            getline(cin, ma);
            if (ma.empty() && !maMacDinh.empty()) return maMacDinh;
            if (repoKH.findById(ma) != nullptr) return ma;
            cout << "Loi: Ma khach hang gui khong ton tai. Hay nhap ma da co trong du lieu KhachHangGui.\n";
        }
    }

    string nhapMaBuuCucHopLe(const string& maMacDinh = "") {
        string ma;
        while (true) {
            cout << "Nhap ma buu cuc";
            if (!maMacDinh.empty()) cout << " [" << maMacDinh << "]";
            cout << ": ";
            getline(cin, ma);
            if (ma.empty() && !maMacDinh.empty()) return maMacDinh;
            if (repoBC.findById(ma) != nullptr) return ma;
            cout << "Loi: Ma buu cuc khong ton tai. Hay nhap ma da co trong du lieu BuuCuc.\n";
        }
    }

public:
    PhieuKhieuNaiMenu(Repository<PhieuKhieuNai>& pkn,
                      Repository<BuuCuc>& bc,
                      Repository<KhachHangGui>& kh)
        : repoPKN(pkn), repoBC(bc), repoKH(kh) {}

    void themMoi() {
        cout << "\n=== THEM PHIEU KHIEU NAI ===\n";
        PhieuKhieuNai pkn;
        string ma;
        while (true) {
            try {
                cout << "Nhap ma phieu khieu nai (VD: PKN01): ";
                getline(cin, ma);
                pkn.setMaID(ma);
                if (repoPKN.findById(ma) != nullptr)
                    throw invalid_argument("Ma phieu khieu nai da ton tai!");
                break;
            } catch (const exception& e) {
                cout << "Loi: " << e.what() << " Vui long nhap lai.\n";
            }
        }

        pkn.setMaKhachHangGui(nhapMaKhachHangHopLe());
        pkn.setMaBuuCuc(nhapMaBuuCucHopLe());
        pkn.nhap();
        repoPKN.add(pkn);
        cout << "-> Them phieu khieu nai thanh cong!\n";
    }

    void xemDanhSach() const {
        auto ds = repoPKN.getAll();
        if (ds.empty()) { cout << "Danh sach phieu khieu nai trong!\n"; return; }
        inTieuDeBang();
        for (const auto& p : ds) p.xuat();
        cout << "Tong so phieu khieu nai: " << ds.size() << '\n';
    }

    void timKiem() const {
        string ma;
        cout << "Nhap ma phieu khieu nai can tim: ";
        getline(cin, ma);
        PhieuKhieuNai* p = repoPKN.findById(ma);
        if (p) { inTieuDeBang(); p->xuat(); }
        else cout << "Khong tim thay phieu khieu nai ma " << ma << "\n";
    }

    void sapXep() {
        auto ds = repoPKN.getAll();
        if (ds.empty()) { cout << "Danh sach phieu khieu nai trong!\n"; return; }
        sort(ds.begin(), ds.end(), [](const PhieuKhieuNai& a, const PhieuKhieuNai& b) {
            return a.getMaID() < b.getMaID();
        });
        inTieuDeBang();
        for (const auto& p : ds) p.xuat();
    }

    void capNhat() {
        string ma;
        cout << "Nhap ma phieu khieu nai can cap nhat: ";
        getline(cin, ma);
        PhieuKhieuNai* cu = repoPKN.findById(ma);
        if (!cu) { cout << "Khong tim thay phieu khieu nai!\n"; return; }

        cout << "De trong de giu nguyen gia tri cu.\n";
        PhieuKhieuNai moi = *cu;
        moi.setMaKhachHangGui(nhapMaKhachHangHopLe(cu->getMaKhachHangGui()));
        moi.setMaBuuCuc(nhapMaBuuCucHopLe(cu->getMaBuuCuc()));

        string temp;
        cout << "Nhap noi dung moi [" << cu->getNoiDung() << "]: ";
        getline(cin, temp);
        if (!temp.empty()) moi.setNoiDung(temp);
        cout << "Nhap trang thai moi [" << cu->getTrangThai() << "]: ";
        getline(cin, temp);
        if (!temp.empty()) moi.setTrangThai(temp);

        if (repoPKN.update(ma, moi)) cout << "-> Cap nhat thanh cong!\n";
        else cout << "Cap nhat that bai!\n";
    }

    void xoa() {
        string ma;
        cout << "Nhap ma phieu khieu nai can xoa: ";
        getline(cin, ma);
        PhieuKhieuNai* p = repoPKN.findById(ma);
        if (!p) { cout << "Khong tim thay phieu khieu nai!\n"; return; }
        inTieuDeBang(); p->xuat();
        cout << "Ban co chac chan muon xoa? (y/n): ";
        char ch; cin >> ch; cin.ignore(10000, '\n');
        if (ch == 'y' || ch == 'Y') {
            if (repoPKN.removeById(ma)) cout << "-> Xoa thanh cong!\n";
        } else cout << "Da huy thao tac xoa.\n";
    }

    void menu() {
        int chon = -1;
        do {
            xoaManHinh();
            cout << "======= QUAN LY PHIEU KHIEU NAI =======\n"
                 << "1. Them phieu khieu nai\n"
                 << "2. Xem danh sach\n"
                 << "3. Tim kiem\n"
                 << "4. Sap xep theo ma\n"
                 << "5. Cap nhat\n"
                 << "6. Xoa\n"
                 << "0. Quay lai menu chinh\n"
                 << "Lua chon: ";
            if (!(cin >> chon)) { cin.clear(); chon = -1; }
            cin.ignore(10000, '\n');
            if (chon == 0) break;
            xoaManHinh();
            switch (chon) {
            case 1: themMoi(); break;
            case 2: xemDanhSach(); break;
            case 3: timKiem(); break;
            case 4: sapXep(); break;
            case 5: capNhat(); break;
            case 6: xoa(); break;
            default: cout << "Lua chon khong hop le!\n";
            }
            tamDung();
        } while (chon != 0);
    }
};

#endif
