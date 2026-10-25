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

using namespace std;

// ============================================================================
// --- LỚP THỰC THỂ TUYENVANCHUYEN ---
// ============================================================================
class TuyenVanChuyen : public Entity {
private:
    string tenTuyen;            // Tên tuyến vận chuyển
    double khoangCach;          // Khoảng cách (km)
    double thoiGianTrungBinh;   // Thời gian vận chuyển trung bình (giờ)

public:
    TuyenVanChuyen() 
        : Entity(), tenTuyen(""), khoangCach(0.0), thoiGianTrungBinh(0.0) {}

    TuyenVanChuyen(string id, string ten, double kc, double tg)
        : Entity(), tenTuyen(ten), khoangCach(kc), thoiGianTrungBinh(tg) {
        this->maID = id;
    }

    // Getters
    string getTenTuyen() const { return tenTuyen; }
    double getKhoangCach() const { return khoangCach; }
    double getThoiGianTrungBinh() const { return thoiGianTrungBinh; }

    // Setters với bắt lỗi hợp lệ dữ liệu
    void setTenTuyen(const string& ten) {
        if (ten.empty()) throw invalid_argument("Ten tuyen khong duoc de trong!");
        tenTuyen = ten;
    }

    void setKhoangCach(double kc) {
        if (kc <= 0) throw invalid_argument("Khoang cach phai lon hon 0 km!");
        khoangCach = kc;
    }

    void setThoiGianTrungBinh(double tg) {
        if (tg <= 0) throw invalid_argument("Thoi gian trung binh phai lon hon 0 gio!");
        thoiGianTrungBinh = tg;
    }

    // --- CÁC PHƯƠNG THỨC TÍNH TOÁN DẪN XUẤT (DERIVED METHODS) ---
    
    // 1. Tính tốc độ di chuyển trung bình (km/h)
    double getTocDoTrungBinh() const {
        if (thoiGianTrungBinh <= 0) return 0.0;
        return khoangCach / thoiGianTrungBinh;
    }

    // 2. Phân loại tuyến vận chuyển dựa trên khoảng cách
    string getPhanLoaiTuyen() const {
        if (khoangCach <= 100) return "Noi tinh / Ngoai thanh";
        if (khoangCach <= 500) return "Lien tinh ngan";
        if (khoangCach <= 1000) return "Lien tinh trung binh";
        return "Tuyen duong dai Bac - Nam";
    }

    // 3. Ước tính cước phí cơ sở dựa trên khoảng cách
    double uocTinhCuocCoBan() const {
        if (khoangCach <= 50) return 15000.0;
        if (khoangCach <= 300) return 15000.0 + (khoangCach - 50) * 200.0;
        return 65000.0 + (khoangCach - 300) * 150.0;
    }

    // Nhập dữ liệu từ bàn phím
    void nhap() override {
        string temp;
        while (true) {
            try {
                cout << "Nhap ten tuyen van chuyen (VD: Ha Noi - Da Nang): ";
                getline(cin, temp);
                setTenTuyen(temp);
                break;
            }
            catch (const exception& e) {
                cout << "Loi: " << e.what() << " Vui long nhap lai.\n";
            }
        }

        while (true) {
            try {
                cout << "Nhap khoang cach vận chuyển (km): ";
                cin >> khoangCach;
                if (cin.fail() || khoangCach <= 0) {
                    cin.clear(); 
                    cin.ignore(10000, '\n');
                    throw invalid_argument("Khoang cach phai la so duong!");
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
                cout << "Nhap thoi gian trung binh (gio): ";
                cin >> thoiGianTrungBinh;
                if (cin.fail() || thoiGianTrungBinh <= 0) {
                    cin.clear(); 
                    cin.ignore(10000, '\n');
                    throw invalid_argument("Thoi gian phai la so duong!");
                }
                cin.ignore(10000, '\n');
                break;
            }
            catch (const exception& e) {
                cout << "Loi: " << e.what() << " Vui long nhap lai.\n";
            }
        }
    }

    // Xuất dạng dòng trong bảng
    void xuat() const override {
        cout << left << setw(10) << maID 
             << "| " << setw(28) << tenTuyen 
             << "| " << setw(15) << fixed << setprecision(1) << khoangCach 
             << "| " << setw(15) << fixed << setprecision(1) << thoiGianTrungBinh 
             << "| " << setw(24) << getPhanLoaiTuyen() << endl;
    }

    // Tuần tự hóa đối tượng để lưu vào File
    string toFileRecord() const override {
        stringstream ss;
        ss << maID << "|" << tenTuyen << "|" << khoangCach << "|" << thoiGianTrungBinh;
        return ss.str();
    }

    // Giải tuần tự hóa từ dòng đọc từ File
    void fromFileRecord(const string& record) override {
        stringstream ss(record);
        string item;
        vector<string> tokens;
        while (getline(ss, item, '|')) tokens.push_back(item);
        if (tokens.size() == 4) {
            maID = tokens[0];
            tenTuyen = tokens[1];
            khoangCach = stod(tokens[2]);
            thoiGianTrungBinh = stod(tokens[3]);
        }
    }
};

// ============================================================================
// --- LỚP GIAO DIỆN VÀ QUẢN LÝ MENU TUYENVANCHUYEN ---
// ============================================================================
class TuyenVanChuyenMenu {
private:
    Repository<TuyenVanChuyen>& repoTVC;
    function<bool(string)> kiemTraTrongHoaDon; // Delegate kiểm tra ràng buộc xóa

    void xoaManHinh() const { system("cls"); }
    void tamDung() const { cout << "\nNhan Enter de tiep tuc..."; cin.get(); }

    void inTieuDeBang() const {
        cout << left << setw(10) << "Ma Tuyen" 
             << "| " << setw(28) << "Ten Tuyen Van Chuyen" 
             << "| " << setw(15) << "Khoang Cach(km)" 
             << "| " << setw(15) << "Thoi Gian(gio)" 
             << "| " << setw(24) << "Phan Loai Tuyen" << endl;
        cout << string(98, '-') << endl;
    }

public:
    TuyenVanChuyenMenu(Repository<TuyenVanChuyen>& tvc, function<bool(string)> checkHD = nullptr) 
        : repoTVC(tvc), kiemTraTrongHoaDon(checkHD) {}

    // 1. Thêm mới tuyến vận chuyển
    void themMoi() {
        cout << "\n=== THEM MOI TUYEN VAN CHUYEN ===\n";
        TuyenVanChuyen tvc;
        string temp;

        while (true) {
            try {
                cout << "Nhap ma tuyen van chuyen (VD: TVC01): ";
                getline(cin, temp);
                tvc.setMaID(temp);
                if (repoTVC.findById(temp) != nullptr) throw invalid_argument("Ma tuyen da ton tai!");
                break;
            }
            catch (const exception& e) {
                cout << "Loi: " << e.what() << " Vui long nhap lai.\n";
            }
        }

        tvc.nhap();
        repoTVC.add(tvc);
        cout << "-> Them moi tuyen van chuyen thanh cong!\n";
    }

    // 2. Xem danh sách tất cả các tuyến
    void xemDanhSach() const {
        auto list = repoTVC.getAll();
        if (list.empty()) {
            cout << "\nDanh sach tuyen van chuyen trong!\n";
            return;
        }
        cout << "\n=================================== DANH SACH TUYEN VAN CHUYEN ===================================\n";
        inTieuDeBang();
        for (const auto& item : list) item.xuat();
        cout << "Tong so tuyen: " << list.size() << endl;
    }

    // 3. Tìm kiếm tuyến theo mã ID
    void timKiem() const {
        string id;
        cout << "Nhap ma tuyen can tim: ";
        getline(cin, id);
        TuyenVanChuyen* result = repoTVC.findById(id);
        if (result != nullptr) {
            cout << "\nDa tim thay thong tin tuyen:\n";
            inTieuDeBang();
            result->xuat();
        }
        else cout << "Khong tim thay tuyen van chuyen ma: " << id << "\n";
    }

    // 4. In báo cáo phiếu thông số kỹ thuật của tuyến
    void inPhieuChiTietTuyen() const {
        string id;
        cout << "Nhap ma tuyen can xem chi tiet: ";
        getline(cin, id);
        TuyenVanChuyen* tvc = repoTVC.findById(id);
        if (tvc == nullptr) {
            cout << "Khong tim thay tuyen ma: " << id << "\n";
            return;
        }

        cout << "\n========================================================\n";
        cout << "           THONG SO CHI TIET TUYEN VAN CHUYEN           \n";
        cout << "========================================================\n";
        cout << " Ma Tuyen Van Chuyen    : " << tvc->getMaID() << "\n";
        cout << " Ten Tuyen              : " << tvc->getTenTuyen() << "\n";
        cout << " Phan Loai Tuyen        : " << tvc->getPhanLoaiTuyen() << "\n";
        cout << "--------------------------------------------------------\n";
        cout << " Khoang Cach Chay       : " << fixed << setprecision(1) << tvc->getKhoangCach() << " km\n";
        cout << " Thoi Gian Van Chuyen TB: " << fixed << setprecision(1) << tvc->getThoiGianTrungBinh() << " gio\n";
        cout << " Toc Do Di Chuyen TB    : " << fixed << setprecision(1) << tvc->getTocDoTrungBinh() << " km/h\n";
        cout << " Cuoc Phi Uoc Tinh Co Ban: " << fixed << setprecision(0) << tvc->uocTinhCuocCoBan() << " VND\n";
        cout << "========================================================\n";
    }

    // 5. Sắp xếp danh sách theo Khoảng cách tăng dần
    void sapXep() {
        auto list = repoTVC.getAll();
        if (list.empty()) {
            cout << "\nDanh sach trong!\n";
            return;
        }

        sort(list.begin(), list.end(), [](const TuyenVanChuyen& a, const TuyenVanChuyen& b) {
            return a.getKhoangCach() < b.getKhoangCach();
        });

        cout << "-> Da sap xep danh sach hien thi theo Khoang Cach (Tang dan)!\n";
        cout << "\n=================================== KET QUA SAP XEP ===================================\n";
        inTieuDeBang();
        for (const auto& item : list) item.xuat();
    }

    // 6. Lọc các tuyến đường theo khoảng cách tối thiểu
    void locTheoKhoangCach() const {
        auto list = repoTVC.getAll();
        double minKm;
        cout << "Nhap khoảng cach toi thieu muon loc (km): ";
        cin >> minKm;
        cin.ignore(10000, '\n');

        int count = 0;
        cout << "\n=========================== KET QUA LOC TUYEN ( > " << minKm << " km) ===========================\n";
        inTieuDeBang();
        for (const auto& item : list) {
            if (item.getKhoangCach() >= minKm) {
                item.xuat();
                count++;
            }
        }
        if (count == 0) cout << "Khong co tuyen nao thoa man dieu kien.\n";
        else cout << "Tong so tuyen tim thay: " << count << endl;
    }

    // 7. Thống kê tổng quan mạng lưới tuyến
    void thongKeTuyen() const {
        auto list = repoTVC.getAll();
        if (list.empty()) {
            cout << "\nKhong co du lieu de thong ke!\n";
            return;
        }

        double tongKhoangCach = 0;
        double tongThoiGian = 0;
        const TuyenVanChuyen* maxTuyen = &list[0];
        const TuyenVanChuyen* minTuyen = &list[0];

        for (const auto& item : list) {
            tongKhoangCach += item.getKhoangCach();
            tongThoiGian += item.getThoiGianTrungBinh();

            if (item.getKhoangCach() > maxTuyen->getKhoangCach()) maxTuyen = &item;
            if (item.getKhoangCach() < minTuyen->getKhoangCach()) minTuyen = &item;
        }

        cout << "\n================ THONG KE MANG LUOI TUYEN VAN CHUYEN ================\n";
        cout << " Tong so tuyen dang quan ly : " << list.size() << "\n";
        cout << " Tong chieu dai mang luoi   : " << fixed << setprecision(1) << tongKhoangCach << " km\n";
        cout << " Khoang cach trung binh/tuyen: " << fixed << setprecision(1) << (tongKhoangCach / list.size()) << " km\n";
        cout << " Thoi gian trung binh/tuyen  : " << fixed << setprecision(1) << (tongThoiGian / list.size()) << " gio\n";
        cout << "--------------------------------------------------------------------\n";
        cout << " Tuyen dai nhat              : " << maxTuyen->getTenTuyen() << " (" << maxTuyen->getKhoangCach() << " km)\n";
        cout << " Tuyen ngan nhat             : " << minTuyen->getTenTuyen() << " (" << minTuyen->getKhoangCach() << " km)\n";
        cout << "====================================================================\n";
    }

    // 8. Cập nhật thông tin tuyến
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

        cout << "Nhap ten tuyen moi [" << old->getTenTuyen() << "]: ";
        getline(cin, temp);
        if (!temp.empty()) newData.setTenTuyen(temp);

        try {
            cout << "Nhap khoang cach moi [" << fixed << setprecision(1) << old->getKhoangCach() << "]: ";
            getline(cin, temp);
            if (!temp.empty() && stod(temp) != -1) newData.setKhoangCach(stod(temp));
        } catch (...) {}

        try {
            cout << "Nhap thoi gian trung binh moi [" << fixed << setprecision(1) << old->getThoiGianTrungBinh() << "]: ";
            getline(cin, temp);
            if (!temp.empty() && stod(temp) != -1) newData.setThoiGianTrungBinh(stod(temp));
        } catch (...) {}

        if (repoTVC.update(id, newData)) cout << "-> Cap nhat thanh cong!\n";
        else cout << "Loi khi cap nhat!\n";
    }

    // 9. Xóa tuyến vận chuyển (Có kiểm tra ràng buộc tham chiếu)
    void xoa() {
        string id;
        cout << "Nhap ma tuyen can xoa: ";
        getline(cin, id);
        TuyenVanChuyen* target = repoTVC.findById(id);
        if (target == nullptr) {
            cout << "Khong tim thay ma nay!\n";
            return;
        }

        // KIỂM TRA RÀNG BUỘC: Không cho xóa nếu mã tuyến đang được dùng trong Hóa đơn
        if (kiemTraTrongHoaDon && kiemTraTrongHoaDon(id)) {
            cout << "-> KHONG THE XOA! Tuyen van chuyen ma [" << id << "] dang duoc tham chieu trong Hoa Don Cuoc.\n";
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

    // Menu điều khiển chức năng
    void menu() {
        int choice = -1;
        do {
            xoaManHinh();
            cout << "========== QUAN LY TUYEN VAN CHUYEN ==========\n";
            cout << "1. Them tuyen van chuyen moi\n";
            cout << "2. Xem danh sach tat ca tuyen\n";
            cout << "3. Tim kiem tuyen theo ma\n";
            cout << "4. In phieu thong so chi tiet tuyen\n";
            cout << "5. Sap xep theo Khoang Cach (Tang dan)\n";
            cout << "6. Loc tuyen theo Khoang Cach toi thieu\n";
            cout << "7. Thong ke mang luoi tuyen van chuyen\n";
            cout << "8. Cap nhat thong tin tuyen\n";
            cout << "9. Xoa tuyen van chuyen\n";
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
            case 4: inPhieuChiTietTuyen(); break;
            case 5: sapXep(); break;
            case 6: locTheoKhoangCach(); break;
            case 7: thongKeTuyen(); break;
            case 8: capNhat(); break;
            case 9: xoa(); break;
            default: cout << "Lua chon khong hop le!\n";
            }
            tamDung();
        } while (choice != 0);
    }
};

#endif
