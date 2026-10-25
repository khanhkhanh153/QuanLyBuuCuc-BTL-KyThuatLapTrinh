#ifndef HOADONCUOC_H
#define HOADONCUOC_H

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
#include "TuyenVanChuyen.h"
#include "LoaiDichVu.h"

using namespace std;

// ============================================================================
// --- LỚP THỰC THỂ HOADONCUOC ---
// ============================================================================
class HoaDonCuoc : public Entity {
private:
    string maTuyen;        // Khóa ngoại tham chiếu TuyenVanChuyen
    string maLoaiDichVu;   // Khóa ngoại tham chiếu LoaiDichVu
    double cuocChinh;
    double phuPhi;
    string trangThaiThanhToan; // "Da thanh toan" hoặc "Chua thanh toan"

public:
    HoaDonCuoc() 
        : Entity(), maTuyen(""), maLoaiDichVu(""), cuocChinh(0.0), phuPhi(0.0), trangThaiThanhToan("Chua thanh toan") {}

    // Getters
    string getMaTuyen() const { return maTuyen; }
    string getMaLoaiDichVu() const { return maLoaiDichVu; }
    double getCuocChinh() const { return cuocChinh; }
    double getPhuPhi() const { return phuPhi; }
    string getTrangThaiThanhToan() const { return trangThaiThanhToan; }

    // Thuộc tính dẫn xuất (Derived Attribute) - Tính toán động
    double getTongTien() const {
        return cuocChinh + phuPhi;
    }

    // Setters
    void setMaTuyen(const string& maT) {
        if (maT.empty()) throw invalid_argument("Ma tuyen van chuyen khong duoc de trong!");
        maTuyen = maT;
    }

    void setMaLoaiDichVu(const string& maDV) {
        if (maDV.empty()) throw invalid_argument("Ma loai dich vu khong duoc de trong!");
        maLoaiDichVu = maDV;
    }

    void setCuocChinh(double cc) {
        if (cc < 0) throw invalid_argument("Cuoc chinh khong duoc am!");
        cuocChinh = cc;
    }

    void setPhuPhi(double pp) {
        if (pp < 0) throw invalid_argument("Phu phi khong duoc am!");
        phuPhi = pp;
    }

    void setTrangThaiThanhToan(const string& tt) {
        if (tt.empty()) throw invalid_argument("Trang thai thanh toan khong duoc de trong!");
        trangThaiThanhToan = tt;
    }

    // Nhập dữ liệu giao diện Console
    void nhap() override {
        while (true) {
            try {
                cout << "Nhap cuoc chinh (VND): ";
                cin >> cuocChinh;
                if (cin.fail() || cuocChinh < 0) {
                    cin.clear(); 
                    cin.ignore(10000, '\n');
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
                    cin.clear(); 
                    cin.ignore(10000, '\n');
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

    // Xuất dạng bảng Console
    void xuat() const override {
        cout << left << setw(12) << maID 
             << "| " << setw(12) << maTuyen 
             << "| " << setw(15) << maLoaiDichVu 
             << "| " << setw(15) << fixed << setprecision(0) << cuocChinh 
             << "| " << setw(12) << fixed << setprecision(0) << phuPhi 
             << "| " << setw(15) << fixed << setprecision(0) << getTongTien() 
             << "| " << setw(18) << trangThaiThanhToan << endl;
    }

    // Chuyển đổi đối tượng thành dòng ghi File
    string toFileRecord() const override {
        stringstream ss;
        ss << maID << "|" << maTuyen << "|" << maLoaiDichVu << "|" << cuocChinh << "|" << phuPhi << "|" << trangThaiThanhToan;
        return ss.str();
    }

    // Đọc từ dòng File vào thuộc tính đối tượng
    void fromFileRecord(const string& record) override {
        stringstream ss(record);
        string item;
        vector<string> tokens;
        while (getline(ss, item, '|')) tokens.push_back(item);
        if (tokens.size() == 6) {
            maID = tokens[0];
            maTuyen = tokens[1];
            maLoaiDichVu = tokens[2];
            cuocChinh = stod(tokens[3]);
            phuPhi = stod(tokens[4]);
            trangThaiThanhToan = tokens[5];
        }
    }
};

// ============================================================================
// --- LỚP GIAO DIỆN VÀ QUẢN LÝ MENU HOADONCUOC ---
// ============================================================================
class HoaDonCuocMenu {
private:
    Repository<HoaDonCuoc>& repoHD;
    Repository<TuyenVanChuyen>& repoTVC;
    Repository<LoaiDichVu>& repoDV;
    function<bool(string)> kiemTraTrongBuuPham; // Lambda kiểm tra ràng buộc không cho xóa nếu còn trong Bưu phẩm

    void xoaManHinh() const { system("cls"); }
    void tamDung() const { cout << "\nNhan Enter de tiep tuc..."; cin.get(); }

    void inTieuDeBang() const {
        cout << left << setw(12) << "Ma Hoa Don" 
             << "| " << setw(12) << "Ma Tuyen" 
             << "| " << setw(15) << "Ma Dich Vu" 
             << "| " << setw(15) << "Cuoc Chinh(VND)" 
             << "| " << setw(12) << "Phu Phi(VND)" 
             << "| " << setw(15) << "Tong Tien(VND)" 
             << "| " << setw(18) << "Trang Thai TT" << endl;
        cout << string(110, '-') << endl;
    }

public:
    HoaDonCuocMenu(Repository<HoaDonCuoc>& hd, 
                   Repository<TuyenVanChuyen>& tvc, 
                   Repository<LoaiDichVu>& dv,
                   function<bool(string)> checkBP = nullptr) 
        : repoHD(hd), repoTVC(tvc), repoDV(dv), kiemTraTrongBuuPham(checkBP) {}

    // 1. Thêm mới Hóa đơn
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

        while (true) {
            try {
                cout << "Nhap ma Tuyen van chuyen (VD: TVC01): ";
                getline(cin, temp);
                if (temp.empty()) throw invalid_argument("Khong duoc de trong!");
                if (repoTVC.findById(temp) == nullptr) {
                    throw invalid_argument("Tuyen van chuyen nay chua ton tai trong danh muc!");
                }
                hd.setMaTuyen(temp);
                break;
            }
            catch (const exception& e) {
                cout << "Loi: " << e.what() << "\n";
            }
        }

        while (true) {
            try {
                cout << "Nhap ma Loai dich vu (VD: DV01): ";
                getline(cin, temp);
                if (temp.empty()) throw invalid_argument("Khong duoc de trong!");
                if (repoDV.findById(temp) == nullptr) {
                    throw invalid_argument("Loai dich vu nay chua ton tai trong danh muc!");
                }
                hd.setMaLoaiDichVu(temp);
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

    // 2. Xem danh sách tất cả Hóa đơn
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

    // 3. Tìm kiếm Hóa đơn theo Mã ID
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

    // 4. In phiếu chi tiết Hóa đơn
    void inPhieuHoaDonChiTiet() const {
        string id;
        cout << "Nhap ma hoa don can in phieu chi tiet: ";
        getline(cin, id);
        HoaDonCuoc* hd = repoHD.findById(id);
        if (hd == nullptr) {
            cout << "Khong tim thay hoa don ma: " << id << "\n";
            return;
        }

        TuyenVanChuyen* tvc = repoTVC.findById(hd->getMaTuyen());
        LoaiDichVu* dv = repoDV.findById(hd->getMaLoaiDichVu());

        cout << "\n========================================================\n";
        cout << "               PHIEU HOA DON CUOC PHI                   \n";
        cout << "========================================================\n";
        cout << " Ma Hoa Don           : " << hd->getMaID() << "\n";
        cout << " Trang Thai Thanh Toan: " << hd->getTrangThaiThanhToan() << "\n";
        cout << "--------------------------------------------------------\n";
        cout << " [1] THONG TIN TUYEN VAN CHUYEN:\n";
        cout << "     - Ma Tuyen       : " << hd->getMaTuyen() << "\n";
        if (tvc != nullptr) {
            cout << "     - Buu Cuc Di     : " << tvc->getMaBuuCucDi() << "\n";
            cout << "     - Buu Cuc Den    : " << tvc->getMaBuuCucDen() << "\n";
            cout << "     - Khoang Cach    : " << fixed << setprecision(1) << tvc->getKhoangCach() << " km\n";
        } else {
            cout << "     - Khong tim thay thong tin chi tiet tuyen!\n";
        }
        cout << "--------------------------------------------------------\n";
        cout << " [2] THONG TIN LOAI DICH VU:\n";
        cout << "     - Ma Dich Vu     : " << hd->getMaLoaiDichVu() << "\n";
        if (dv != nullptr) {
            cout << "     - Ten Dich Vu    : " << dv->getTenDichVu() << "\n";
        } else {
            cout << "     - Khong tim thay thong tin chi tiet dich vu!\n";
        }
        cout << "--------------------------------------------------------\n";
        cout << " [3] CHI TIET CUOC PHI:\n";
        cout << "     - Cuoc Chinh     : " << fixed << setprecision(0) << hd->getCuocChinh() << " VND\n";
        cout << "     - Phu Phi        : " << fixed << setprecision(0) << hd->getPhuPhi() << " VND\n";
        cout << "     => TONG TIEN     : " << fixed << setprecision(0) << hd->getTongTien() << " VND\n";
        cout << "========================================================\n";
    }

    // 5. Sắp xếp danh sách theo Tổng tiền giảm dần
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

    // 6. Lọc hóa đơn chưa thanh toán
    void locChuaThanhToan() const {
        auto list = repoHD.getAll();
        int count = 0;
        cout << "\n=========================== DANH SACH HOA DON CHUA THANH TOAN ===========================\n";
        inTieuDeBang();
        for (const auto& item : list) {
            if (item.getTrangThaiThanhToan() == "Chua thanh toan") {
                item.xuat();
                count++;
            }
        }
        if (count == 0) cout << "Khong co hoa don nao chua thanh toan.\n";
        else cout << "Tong so hoa don chua thanh toan: " << count << endl;
    }

    // 7. Thống kê tổng doanh thu
    void thongKeDoanhThu() const {
        auto list = repoHD.getAll();
        double tongDaThu = 0;
        double tongChuaThu = 0;
        int daThuCount = 0;

        for (const auto& item : list) {
            if (item.getTrangThaiThanhToan() == "Da thanh toan") {
                tongDaThu += item.getTongTien();
                daThuCount++;
            } else {
                tongChuaThu += item.getTongTien();
            }
        }

        cout << "\n================ THONG KE DOANH THU CUOC PHI ================\n";
        cout << " Tong so hoa don         : " << list.size() << "\n";
        cout << " So hoa don da thanh toan: " << daThuCount << "\n";
        cout << " Tong doanh thu da thu   : " << fixed << setprecision(0) << tongDaThu << " VND\n";
        cout << " Tong cuoc phi chua thu  : " << fixed << setprecision(0) << tongChuaThu << " VND\n";
        cout << " Tong gia tri cuoc phi   : " << fixed << setprecision(0) << (tongDaThu + tongChuaThu) << " VND\n";
        cout << "=============================================================\n";
    }

    // 8. Cập nhật thông tin Hóa đơn
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

        cout << "Nhap ma Loai dich vu moi [" << old->getMaLoaiDichVu() << "]: ";
        getline(cin, temp);
        if (!temp.empty()) {
            if (repoDV.findById(temp) == nullptr) cout << "Loi: Loai dich vu khong ton tai! Giu nguyen ma cu.\n";
            else newData.setMaLoaiDichVu(temp);
        }

        try {
            cout << "Nhap cuoc chinh moi [" << fixed << setprecision(0) << old->getCuocChinh() << "]: ";
            getline(cin, temp);
            if (!temp.empty() && stod(temp) != -1) newData.setCuocChinh(stod(temp));
        } catch (...) {}

        try {
            cout << "Nhap phu phi moi [" << fixed << setprecision(0) << old->getPhuPhi() << "]: ";
            getline(cin, temp);
            if (!temp.empty() && stod(temp) != -1) newData.setPhuPhi(stod(temp));
        } catch (...) {}

        cout << "Nhap trang thai thanh toan moi (1 - Da thanh toan, 2 - Chua thanh toan) [" << old->getTrangThaiThanhToan() << "]: ";
        getline(cin, temp);
        if (temp == "1") newData.setTrangThaiThanhToan("Da thanh toan");
        else if (temp == "2") newData.setTrangThaiThanhToan("Chua thanh toan");

        if (repoHD.update(id, newData)) cout << "-> Cap nhat thanh cong!\n";
        else cout << "Loi khi cap nhat!\n";
    }

    // 9. Xóa Hóa đơn (Có kiểm tra ràng buộc không được xóa nếu còn trong Bưu phẩm)
    void xoa() {
        string id;
        cout << "Nhap ma hoa don can xoa: ";
        getline(cin, id);
        HoaDonCuoc* target = repoHD.findById(id);
        if (target == nullptr) {
            cout << "Khong tim thay ma nay!\n";
            return;
        }

        // KIỂM TRA RÀNG BUỘC: Không cho xóa nếu mã hóa đơn đang có trong Bưu phẩm
        if (kiemTraTrongBuuPham && kiemTraTrongBuuPham(id)) {
            cout << "-> KHONG THE XOA! Hoa don cuoc ma [" << id << "] dang duoc tham chieu trong Buu Pham.\n";
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
            cout << "========== QUAN LY HOA DON CUOC PHI ==========\n";
            cout << "1. Them hoa don cuoc moi\n";
            cout << "2. Xem danh sach tat ca hoa don\n";
            cout << "3. Tim kiem hoa don theo ma\n";
            cout << "4. In phieu chi tiet hoa don (Gom Tuyen & Dich Vu)\n";
            cout << "5. Sap xep danh sach theo Tong Tien (Giam dan)\n";
            cout << "6. Loc danh sach hoa don CHUA thanh toan\n";
            cout << "7. Thong ke tong doanh thu\n";
            cout << "8. Cap nhat hoa don\n";
            cout << "9. Xoa hoa don\n";
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
            case 4: inPhieuHoaDonChiTiet(); break;
            case 5: sapXep(); break;
            case 6: locChuaThanhToan(); break;
            case 7: thongKeDoanhThu(); break;
            case 8: capNhat(); break;
            case 9: xoa(); break;
            default: cout << "Lua chon khong hop le!\n";
            }
            tamDung();
        } while (choice != 0);
    }
};

#endif