//Nguyễn Mạnh Tú

#ifndef PHIEUGUI_H
#define PHIEUGUI_H

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
#include "BuuCuc.h"
#include "KhachHangGui.h"
#include "BuuPham.h" 

using namespace std;

// --- LOP THUC THE ---
class PhieuGui : public Entity {
private:
    string maBuuCucGui;
    string maKhachHang;
    string maBuuPham;

public:
    PhieuGui() : Entity(), maBuuCucGui(""), maKhachHang(""), maBuuPham("") {}

    string getMaBuuCucGui() const 
    { 
        return maBuuCucGui; 
    }
    void setMaBuuCucGui(const string& maBC) {
        if (maBC.empty()) throw invalid_argument("Ma buu cuc khong duoc de trong!");
        maBuuCucGui = maBC;
    }

    string getMaKhachHang() const 
    { 
        return maKhachHang;
    }
    void setMaKhachHang(const string& maKH) 
    {
        if (maKH.empty()) throw invalid_argument("Ma khach hang khong duoc de trong!");
        maKhachHang = maKH;
    }

    string getMaBuuPham() const 
    { 
        return maBuuPham; 
    }
    void setMaBuuPham(const string& maBP) 
    {
        if (maBP.empty()) throw invalid_argument("Ma buu pham khong duoc de trong!");
        maBuuPham = maBP;
    }

    void nhap() override 
    {
        // Cac thuoc tinh deu la khoa ngoai, viec nhap/kiem tra do Menu xu ly
    }

    void xuat() const override 
    {
        cout << left << setw(10) << maID
            << "| " << setw(15) << maBuuCucGui
            << "| " << setw(15) << maKhachHang
            << "| " << setw(15) << maBuuPham << endl;
    }

    string toFileRecord() const override 
    {
        stringstream ss;
        ss << maID << "|" << maBuuCucGui << "|" << maKhachHang << "|" << maBuuPham;
        return ss.str();
    }

    void fromFileRecord(const string& record) override 
    {
        stringstream ss(record);
        string item;
        vector<string> tokens;
        while (getline(ss, item, '|')) tokens.push_back(item);
        if (tokens.size() == 4) 
        {
            maID = tokens[0];
            maBuuCucGui = tokens[1];
            maKhachHang = tokens[2];
            maBuuPham = tokens[3];
        }
    }
};

// --- LOP GIAO DIEN (MENU) ---
class PhieuGuiMenu 
{
private:
    Repository<PhieuGui>& repoPG;
    Repository<BuuCuc>& repoBC;
    Repository<KhachHangGui>& repoKH;
    Repository<BuuPham>& repoBP;

    void xoaManHinh() const { system("cls"); }
    void tamDung() const { 
        cout << "\nNhan Enter de tiep tuc..."; 
        cin.get(); 
    }
    void inTieuDeBang() const {
        cout << left << setw(10) << "Ma Phieu" << "| " << setw(15) << "Ma Buu Cuc"
            << "| " << setw(15) << "Ma Khach Hang" << "| " << setw(15) << "Ma Buu Pham" << endl;
        cout << string(60, '-') << endl;
    }

public:
    PhieuGuiMenu(Repository<PhieuGui>& pg, Repository<BuuCuc>& bc, Repository<KhachHangGui>& kh, Repository<BuuPham>& bp)
        : repoPG(pg), repoBC(bc), repoKH(kh), repoBP(bp) {
    }

    void themMoi() {
        cout << "\n=== THEM MOI PHIEU GUI ===\n";
        PhieuGui pg; string temp;

        while (true) 
        {
            try {
                cout << "Nhap ma phieu gui (VD: PG01): "; getline(cin, temp);
                pg.setMaID(temp);
                if (repoPG.findById(temp) != nullptr) throw invalid_argument("Ma phieu gui da ton tai!");
                break;
            }
            catch (const exception& e) { cout << "Loi: " << e.what() << "\n"; }
        }

        while (true) 
        {
            try {
                cout << "Nhap ma Buu cuc gui (VD: BC01): "; getline(cin, temp);
                if (temp.empty()) throw invalid_argument("Khong duoc de trong!");
                if (repoBC.findById(temp) == nullptr) throw invalid_argument("Buu cuc chua ton tai!");
                pg.setMaBuuCucGui(temp);
                break;
            }
            catch (const exception& e) { cout << "Loi: " << e.what() << "\n"; }
        }

        while (true) 
        {
            try {
                cout << "Nhap ma Khach hang gui (VD: KH01): "; getline(cin, temp);
                if (temp.empty()) throw invalid_argument("Khong duoc de trong!");
                if (repoKH.findById(temp) == nullptr) throw invalid_argument("Khach hang chua ton tai!");
                pg.setMaKhachHang(temp);
                break;
            }
            catch (const exception& e) { cout << "Loi: " << e.what() << "\n"; }
        }

        while (true) 
        {
            try {
                cout << "Nhap ma Buu pham (VD: BP01): "; getline(cin, temp);
                if (temp.empty()) throw invalid_argument("Khong duoc de trong!");
                if (repoBP.findById(temp) == nullptr) throw invalid_argument("Buu pham chua ton tai!");
                pg.setMaBuuPham(temp);
                break;
            }
            catch (const exception& e) { cout << "Loi: " << e.what() << "\n"; }
        }

        repoPG.add(pg);
        cout << "-> Them moi thanh cong!\n";
    }

    void xemDanhSach() const 
    {
        auto list = repoPG.getAll();
        if (list.empty()) { cout << "\nDanh sach trong!\n"; return; }
        cout << "\n=================== DANH SACH PHIEU GUI ===================\n";
        inTieuDeBang();
        for (const auto& item : list) item.xuat();
        cout << "Tong so phieu gui: " << list.size() << endl;
    }

    void timKiem() const 
    {
        string id; cout << "Nhap ma phieu gui can tim: "; getline(cin, id);
        PhieuGui* result = repoPG.findById(id);
        if (result != nullptr) {
            cout << "\nDa tim thay thong tin:\n"; inTieuDeBang(); result->xuat();
        }
        else cout << "Khong tim thay phieu gui ma: " << id << "\n";
    }

    void sapXep() 
    {
        auto list = repoPG.getAll();
        if (list.empty()) { cout << "\nDanh sach trong!\n"; return; }

        sort(list.begin(), list.end(), [](const PhieuGui& a, const PhieuGui& b) {
            return a.getMaKhachHang() < b.getMaKhachHang();
            });

        cout << "-> Da sap xep danh sach hien thi theo Ma Khach Hang (A-Z)!\n";
        cout << "\n=================== KET QUA SAP XEP ===================\n";
        inTieuDeBang();
        for (const auto& item : list) item.xuat();
    }

    void capNhat() 
    {
        string id; cout << "Nhap ma phieu can cap nhat: "; getline(cin, id);
        PhieuGui* old = repoPG.findById(id);
        if (old == nullptr) { cout << "Khong tim thay ma nay!\n"; return; }

        cout << "\nThong tin hien tai:\n"; inTieuDeBang(); old->xuat();
        cout << "\n(Luu y: De trong neu muon giu nguyen gia tri cu)\n";

        PhieuGui newData = *old; string temp;

        cout << "Nhap ma Buu cuc moi [" << old->getMaBuuCucGui() << "]: "; getline(cin, temp);
        if (!temp.empty()) {
            if (repoBC.findById(temp) == nullptr) cout << "Loi: Buu cuc chua ton tai! Giu nguyen ma cu.\n";
            else newData.setMaBuuCucGui(temp);
        }

        cout << "Nhap ma Khach hang moi [" << old->getMaKhachHang() << "]: "; getline(cin, temp);
        if (!temp.empty()) {
            if (repoKH.findById(temp) == nullptr) cout << "Loi: Khach hang chua ton tai! Giu nguyen ma cu.\n";
            else newData.setMaKhachHang(temp);
        }

        cout << "Nhap ma Buu pham moi [" << old->getMaBuuPham() << "]: "; getline(cin, temp);
        if (!temp.empty()) {
            if (repoBP.findById(temp) == nullptr) cout << "Loi: Buu pham chua ton tai! Giu nguyen ma cu.\n";
            else newData.setMaBuuPham(temp);
        }

        if (repoPG.update(id, newData)) cout << "-> Cap nhat thanh cong!\n";
        else cout << "Loi khi cap nhat!\n";
    }

    void xoa() 
    {
        string id; cout << "Nhap ma phieu gui can xoa: "; getline(cin, id);
        if (repoPG.findById(id) == nullptr) { cout << "Khong tim thay ma nay!\n"; return; }

        cout << "Ban co chac chan muon xoa khong? (y/n): ";
        char confirm; cin >> confirm; cin.ignore(10000, '\n');
        if (confirm == 'y' || confirm == 'Y') {
            if (repoPG.removeById(id)) cout << "-> Xoa thanh cong!\n";
        }
        else cout << "Huy thao tac xoa.\n";
    }

    void menu() {
        int choice = -1;
        do {
            xoaManHinh();
            cout << "========== QUAN LY PHIEU GUI ==========\n";
            cout << "1. Them phieu gui moi\n2. Xem danh sach\n3. Tim kiem\n4. Sap xep theo Ma KH\n5. Cap nhat\n6. Xoa\n0. Quay lai menu chinh\n";
            cout << "=======================================\nChon: ";
            if (!(cin >> choice)) {
                cin.clear(); 
                choice = -1; 
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
