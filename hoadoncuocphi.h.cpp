#ifndef APPLICATION_H
#define APPLICATION_H

#include <iostream>
#include <memory>
#include <cstdlib>

#include "Repository.h"
#include "BuuCuc.h"
#include "PhieuGui.h"
#include "HoaDonCuoc.h"
#include "TuyenVanChuyen.h"
#include "LoaiDichVu.h"

using namespace std;

class Application {
private:
    Repository<BuuCuc> repoBC;
    Repository<PhieuGui> repoPG;
    Repository<HoaDonCuoc> repoHD;
    Repository<TuyenVanChuyen> repoTVC;
    Repository<LoaiDichVu> repoDV;

    unique_ptr<BuuCucMenu> menuBC;
    unique_ptr<PhieuGuiMenu> menuPG;
    unique_ptr<HoaDonCuocMenu> menuHD;
    unique_ptr<TuyenVanChuyenMenu> menuTVC;

    void xoaManHinh() const { system("cls"); }

public:
    Application()
        : repoBC("data/buu_cuc.txt"),
          repoPG("data/phieu_gui.txt"),
          repoHD("data/hoa_don.txt"),
          repoTVC("data/tuyen_van_chuyen.txt"),
          repoDV("data/loai_dich_vu.txt")
    {
        auto kiemTraBC_trong_PG = [&](string maBC) {
            auto list = repoPG.getAll();
            for (const auto& pg : list) {
                if (pg.getMaBuuCucGui() == maBC) return true;
            }
            return false;
        };

        // Lambda kiểm tra tuyến vận chuyển có nằm trong Hóa đơn cước không (Được 2 use case tham chiếu)
        auto kiemTraTVC_trong_HD = [&](string maTuyen) {
            auto list = repoHD.getAll();
            for (const auto& hd : list) {
                if (hd.getMaTuyen() == maTuyen) return true;
            }
            return false;
        };

        menuBC = make_unique<BuuCucMenu>(repoBC, kiemTraBC_trong_PG);
        menuPG = make_unique<PhieuGuiMenu>(repoPG, repoBC);
        menuHD = make_unique<HoaDonCuocMenu>(repoHD, repoTVC, repoDV); // Truyền thêm repoDV
        menuTVC = make_unique<TuyenVanChuyenMenu>(repoTVC, repoBC, kiemTraTVC_trong_HD);
    }

    void run() {
        int choice = -1;
        do {
            xoaManHinh();
            cout << "================ HE THONG QUAN LY BUU CUC ================\n";
            cout << "1. Quan ly Buu Cuc\n";
            cout << "2. Quan ly Phieu Gui\n";
            cout << "3. Quan ly Cuoc Phi - Hoa Don\n";
            cout << "4. Quan ly Tuyen Van Chuyen\n";
            cout << "0. Thoat chuong trinh\n";
            cout << "==========================================================\n";
            cout << "Nhap lua chon phan he: ";

            if (!(cin >> choice)) {
                cin.clear();
                cin.ignore(10000, '\n'); 
                choice = -1; 
                continue;
            }

            switch (choice) {
            case 1: menuBC->menu(); break; 
            case 2: menuPG->menu(); break;
            case 3: menuHD->menu(); break;
            case 4: menuTVC->menu(); break;
            case 0:
                cout << "Dang thoat chuong trinh va sao luu du lieu...\n";
                break;
            default: cout << "Lua chon khong hop le! Vui long chon lai.\n";
            }
        } while (choice != 0);
    }
};

#endif