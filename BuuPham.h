// Nguyễn Như Khánh
#ifndef BUUPHAM_H
#define BUUPHAM_H

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>// Nguyễn Như Khánh
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
#include "HoaDonCuocPhi.h"

using namespace std;

class BuuPham : public Entity {
private:
    string tenBuuPham;
    double trongLuong;
    int soLuong;       // Thay thế cho maBuuCucGui bằng số lượng bưu phẩm
    string maHoaDon;   // Khóa ngoại tham chiếu sang Hóa đơn

public:
    BuuPham() : Entity(), tenBuuPham(""), trongLuong(0.0), soLuong(1), maHoaDon("") {}

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

    int getSoLuong() const { return soLuong; }
    void setSoLuong(int sl) {
        if (sl <= 0) throw invalid_argument("So luong phai lon hon 0!");
        soLuong = sl;
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
        while (true) {
            try {
                cout << "Nhap so luong: "; cin >> soLuong;
                if (cin.fail() || soLuong <= 0) {
                    cin.clear(); cin.ignore(10000, '\n');
                    throw invalid_argument("So luong phai la so nguyen lon hon 0!");
                }
                cin.ignore(); break;
            }
            catch (const exception& e) { cout << "Loi: " << e.what() << "\n"; }
        }
    }

    void xuat() const override {
        cout << left << setw(10) << maID << "| " << setw(35) << tenBuuPham
             << "| " << setw(18) << fixed << setprecision(1) << trongLuong 
             << "| " << setw(8) << soLuong 
             << "| " << setw(10) << maHoaDon << endl;
    }

    string toFileRecord() const override {
        stringstream ss;
        ss << maID << "|" << tenBuuPham << "|" << trongLuong << "|" << soLuong << "|" << maHoaDon;
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
            soLuong = stoi(tokens[3]);
            maHoaDon = tokens[4];
        }
    }
};

class BuuPhamMenu {
private:
    Repository<BuuPham>& repoBP;
    Repository<HoaDonCuoc>& repoHD; 
    
    function<bool(string)> kiemTraRangBuocPhieuGui;

    void xoaManHinh() const { system("cls"); }
    void tamDung() const { cout << "\nNhan Enter de tiep tuc..."; cin.get(); }
    void inTieuDeBang() const {
        cout << left << setw(10) << "Ma BP" << "| " << setw(35) << "Ten Buu Pham"
             << "| " << setw(18) << "Trong Luong" << "| " << setw(8) << "So Luong" << "| " << setw(10) << "Ma HD" << endl;
        cout << string(80, '-') << endl;
    }

public:
    BuuPhamMenu(Repository<BuuPham>& bp, Repository<HoaDonCuoc>& hd, function<bool(string)> kiemTraPG = nullptr) 
        : repoBP(bp), repoHD(hd), kiemTraRangBuocPhieuGui(kiemTraPG) {}

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
            return a.getSoLuong() < b.getSoLuong();
        });

        cout << "-> Da sap xep danh sach theo So Luong (Tang dan)!\n";
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

        cout << "Nhap so luong moi [" << old->getSoLuong() << "]: "; getline(cin, temp);
        if (!temp.empty()) newData.setSoLuong(stoi(temp));

        cout << "Nhap ma hoa don moi [" << old->getMaHoaDon() << "]: "; getline(cin, temp);
        if (!temp.empty()) {
            if (repoHD.findById(temp) == nullptr) {
                cout << "Loi: Hoa don khong ton tai! Giu nguyen ma cu.\n";
            } else {
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

        cout << "\nThunh tin chuan bi xoa:\n"; inTieuDeBang(); target->xuat();
        
        if (kiemTraRangBuocPhieuGui != nullptr && kiemTraRangBuocPhieuGui(id)) {
            cout << "\n[LOI RANG BUOC DU LIEU] KHONG THE XOA!\n";
            cout << "-> Buu pham nay dang duoc tham chieu trong Phieu gui.\n";
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
            cout << "4. Sap xep theo So Luong\n";
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

#include <algorithm>
#include <stdexcept>
#include <functional>
#include "Entity.h"
#include "Repository.h"
#include "HoaDonCuocPhi.h"

using namespace std;

class BuuPham : public Entity {
private:
    string tenBuuPham;
    double trongLuong;
    int soLuong;       // Thay thế cho maBuuCucGui bằng số lượng bưu phẩm
    string maHoaDon;   // Khóa ngoại tham chiếu sang Hóa đơn

public:
    BuuPham() : Entity(), tenBuuPham(""), trongLuong(0.0), soLuong(1), maHoaDon("") {}

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

    int getSoLuong() const { return soLuong; }
    void setSoLuong(int sl) {
        if (sl <= 0) throw invalid_argument("So luong phai lon hon 0!");
        soLuong = sl;
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
        while (true) {
            try {
                cout << "Nhap so luong: "; cin >> soLuong;
                if (cin.fail() || soLuong <= 0) {
                    cin.clear(); cin.ignore(10000, '\n');
                    throw invalid_argument("So luong phai la so nguyen lon hon 0!");
                }
                cin.ignore(); break;
            }
            catch (const exception& e) { cout << "Loi: " << e.what() << "\n"; }
        }
    }

    void xuat() const override {
        cout << left << setw(8) << maID << "| " << setw(18) << tenBuuPham
             << "| " << setw(10) << fixed << setprecision(1) << trongLuong 
             << "| " << setw(8) << soLuong 
             << "| " << setw(10) << maHoaDon << endl;
    }

    string toFileRecord() const override {
        stringstream ss;
        ss << maID << "|" << tenBuuPham << "|" << trongLuong << "|" << soLuong << "|" << maHoaDon;
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
            soLuong = stoi(tokens[3]);
            maHoaDon = tokens[4];
        }
    }
};

class BuuPhamMenu {
private:
    Repository<BuuPham>& repoBP;
    Repository<HoaDonCuoc>& repoHD; 
    
    function<bool(string)> kiemTraRangBuocPhieuGui;

    void xoaManHinh() const { system("cls"); }
    void tamDung() const { cout << "\nNhan Enter de tiep tuc..."; cin.get(); }
    void inTieuDeBang() const {
        cout << left << setw(8) << "Ma BP" << "| " << setw(18) << "Ten Buu Pham"
             << "| " << setw(10) << "Trong Luong" << "| " << setw(8) << "So Luong" << "| " << setw(10) << "Ma HD" << endl;
        cout << string(62, '-') << endl;
    }

public:
    BuuPhamMenu(Repository<BuuPham>& bp, Repository<HoaDonCuoc>& hd, function<bool(string)> kiemTraPG = nullptr) 
        : repoBP(bp), repoHD(hd), kiemTraRangBuocPhieuGui(kiemTraPG) {}

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
            return a.getSoLuong() < b.getSoLuong();
        });

        cout << "-> Da sap xep danh sach theo So Luong (Tang dan)!\n";
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

        cout << "Nhap so luong moi [" << old->getSoLuong() << "]: "; getline(cin, temp);
        if (!temp.empty()) newData.setSoLuong(stoi(temp));

        cout << "Nhap ma hoa don moi [" << old->getMaHoaDon() << "]: "; getline(cin, temp);
        if (!temp.empty()) {
            if (repoHD.findById(temp) == nullptr) {
                cout << "Loi: Hoa don khong ton tai! Giu nguyen ma cu.\n";
            } else {
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

        cout << "\nThunh tin chuan bi xoa:\n"; inTieuDeBang(); target->xuat();
        
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
            cout << "4. Sap xep theo So Luong\n";
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
