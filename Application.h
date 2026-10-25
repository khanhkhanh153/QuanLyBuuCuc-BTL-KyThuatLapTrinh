#ifndef APPLICATION_H
#define APPLICATION_H

#include <iostream>
#include <memory>
#include <cstdlib>

#include "Repository.h"
#include "BuuCuc.h"
#include "PhieuGui.h"
#include "BuuPham.h"
#include "LoaiDichVu.h"
#include "TuyenVanChuyen.h"
#include "HoaDonCuoc.h"
#include "KhachHangGui.h"
#include "PhieuKhieuNai.h"

using namespace std;

class Application {
private:
    // 1. KHOI TAO TRUY TIEP BẰNG NGOAC NHON {} (C++11)
    Repository<BuuCuc> repoBC{ "data/buu_cuc.txt" };
    Repository<PhieuGui> repoPG{ "data/phieu_gui.txt" };
    Repository<BuuPham> repoBP{ "data/buu_pham.txt" };
    Repository<LoaiDichVu> repoLDV{ "data/loai_dich_vu.txt" };
    Repository <TuyenVanChuyen> repoTVC{ "data/tuyen_van_chuyen.txt" };
    Repository <HoaDonCuoc> repoHD{ "data/hoa_don_cuoc_phi.txt" };
    Repository<KhachHangGui> repoKH{ "data/khach_hang_gui.txt" };
    Repository<PhieuKhieuNai> repoPKN{ "data/phieu_khieu_nai.txt" };

    // 2. CAC CON TRO QUAN LY MENU
    unique_ptr<BuuCucMenu> menuBC;
    unique_ptr<PhieuGuiMenu> menuPG;
    unique_ptr<BuuPhamMenu> menuBP;
    unique_ptr<LoaiDichVuMenu> menuLDV;
    unique_ptr<TuyenVanChuyenMenu> menuTVC;
    unique_ptr<HoaDonCuocMenu> menuHD;
    unique_ptr<KhachHangGuiMenu> menuKH;
    unique_ptr<PhieuKhieuNaiMenu> menuPKN;

    void xoaManHinh() const { system("cls"); }

public:
    // CONSTRUCTOR: Khong can danh sach khoi tao dai dong phia duoi
    Application() {
        // Các lambda kiểm tra ràng buộc
        auto kiemTraBC_trong_PG = [&](string maBC) {
            // Kiem tra trong Phieu Gui
            auto listPG = repoPG.getAll();
            for (const auto& pg : listPG) {
                if (pg.getMaBuuCucGui() == maBC) return true;
            }

            auto listBC = repoPKN.getAll();
            for (const auto& pkn : listBC) {
                if (pkn.getMaBuuCuc() == maBC) 
                    return true;
            }
            return false;
        }

           // Lambda kiểm tra Loại dịch vụ có trong Hóa đơn không
        auto kiemTraLDV_trong_HD = [&](string maDV) {
            auto listHD = repoHD.getAll();
            for (const auto& hd : listHD) {
                if (hd.getMaDichVu() == maDV) return true;
            }
            return false;
        }

        // Lambda kiểm tra Bưu phẩm có trong Phiếu gửi không
        auto kiemTraBP_trong_PG = [&](string maBP) {
            auto listPG = repoPG.getAll();
            for (const auto& pg : listPG) {
                if(pg.getMaBuuPham() == maBP) return true;
            }
            return false;
        }

        auto kiemTraTVC_trong_HD = [&](string maHD) {
            auto list = repoHD.getAll();
            for (const auto& hd : list) {
                if (hd.getMaTuyen() == maHD) return true;
            }
            return false;
        };

        auto kiemTraHD_trong_BP = [&](string maHD) {
            auto list = repoBP.getAll();
            for (const auto& bp : list) {
                if (bp.getMaHoaDon() == maHD) return true;
            }
            return false;
        };


        auto kiemTraXoaKhachHang = [&](string maKH) {
            auto list = repoPKN.getAll();
            for (const auto& pkn : list) {
                if (pkn.getMaKhachHang() == maKH) return true;
            }
            auto listKH = repoPG.getAll();
            for (const auto& pg : list) {
                if (pg.getMaKhachHang() == maKH) return true;
            }
            return false;
        };

        // Khoi tao Menu
        menuBC = make_unique<BuuCucMenu>(repoBC, kiemTraBC_trong_PG);
        enuPG = make_unique<PhieuGuiMenu>(repoPG, repoBC, repoKH, repoBP);
        menuLDV = make_unique<LoaiDichVuMenu>(repoLDV, kiemTraLDV_trong_BP);
        menuBP = make_unique<BuuPhamMenu>(repoBP, repoLDV);
        menuTVC = make_unique<TuyenVanChuyenMenu>(repoTVC, kiemTraTVC_trong_HD);
        menuHD = make_unique<HoaDonCuocMenu>(repoHD, repoTVC, repoLDV, kiemTraHD_trong_BP);
        menuKH = make_unique<KhachHangGuiMenu>(repoKH, kiemTraXoaKhachHang);
        menuPKN = make_unique<PhieuKhieuNaiMenu>(repoPKN, repoBC, repoKH);
    }

    void run() {
        int choice = -1;
        do {
            xoaManHinh();
            cout << "================ HE THONG QUAN LY BUU CUC ================\n";
            cout << "1. Quan ly Buu Cuc\n";
            cout << "2. Quan ly Phieu Gui\n";
            cout << "3. Quan ly Buu Pham\n";
            cout << "4. Quan ly Loai Dich Vu\n";
            cout << "5. Quan ly Tuyen Van Chuyen\n";
            cout << "6. Quan ly Hoa Don Cuoc Phi\n";
            cout << "7. Quan ly Khach Hang Gui\n";
            cout << "8. Quan ly Phieu Khieu Nai\n";           
            cout << "0. Thoat chuong trinh\n";
            cout << "==========================================================\n";
            cout << "Nhap lua chon phan he (0-8): ";

            if (!(cin >> choice)) {
                cin.clear();
                cin.ignore(10000, '\n');
                choice = -1;
                continue;
            }

            switch (choice) {
            case 1: menuBC->menu(); break;
            case 2: menuPG->menu(); break;
            case 3: menuBP->menu(); break;
            case 4: menuLDV->menu(); break;
            case 5:menuTVC->menu(); break;
            case 6:menuHD->menu(); break;
            case 7:menuKH->menu(); break;
            case 8:menuPKN->menu(); break;
            case 0:
                cout << "Dang thoat chuong trinh va sao luu du lieu...\n";
                break;
            default: cout << "Lua chon khong hop le!\n";
            }
        } while (choice != 0);
    }
};

#endif
