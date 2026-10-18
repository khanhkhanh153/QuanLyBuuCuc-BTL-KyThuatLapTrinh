#ifndef APPLICATION_H
#define APPLICATION_H

#include <iostream>
#include <memory> // Thư viện dùng Smart Pointer (unique_ptr)
#include <cstdlib>

#include "Repository.h"
#include "BuuCuc.h"
#include "PhieuGui.h"
#include "BuuPham.h"
#include "LoaiDichVu.h"

using namespace std;

class Application {
private:
    // 1. Khai báo Repository cho các phân hệ
    Repository<BuuCuc> repoBC;
    Repository<PhieuGui> repoPG;
    Repository<BuuPham> repoBP;
    Repository<LoaiDichVu> repoLDV;

    // 2. Sử dụng Smart Pointer để quản lý Menu các phân hệ
    unique_ptr<BuuCucMenu> menuBC;
    unique_ptr<PhieuGuiMenu> menuPG;
    unique_ptr<BuuPhamMenu> menuBP;
    unique_ptr<LoaiDichVuMenu> menuLDV;

    void xoaManHinh() const { system("cls"); }

public:
    // CONSTRUCTOR: Khởi tạo và thiết lập các ràng buộc tham chiếu chéo
    Application()
        : repoBC("data/buu_cuc.txt"),
          repoPG("data/phieu_gui.txt"),
          repoBP("data/buu_pham.txt"),
          repoLDV("data/loai_dich_vu.txt")
    {
        // Khởi tạo các Menu và tiêm (inject) các Repository vào
        menuBC = make_unique<BuuCucMenu>(repoBC);
        menuPG = make_unique<PhieuGuiMenu>(repoPG, repoBC);
        menuBP = make_unique<BuuPhamMenu>(repoBP, repoBC, repoLDV);
        menuLDV = make_unique<LoaiDichVuMenu>(repoLDV);
    }

    // HÀM CHẠY MENU CHÍNH
    void run() {
        int choice = -1;
        do {
            xoaManHinh();
            cout << "=================== MENU CHINH QUAN LY BUU CUC ===================\n";
            cout << "1. Quan ly Buu Cuc\n";
            cout << "2. Quan ly Phieu Gui\n";
            cout << "3. Quan ly Buu Pham\n";
            cout << "4. Quan ly Loai Dich Vu\n";
            cout << "0. Thoat chuong trinh\n";
            cout << "=================================================================\n";
            cout << "Chon chuc nang (0-4): ";

            if (!(cin >> choice)) {
                cin.clear();
                cin.ignore(10000, '\n'); 
                choice = -1; 
                continue;
            }

            switch (choice) {
            case 1: 
                menuBC->menu(); 
                break; 
            case 2: 
                menuPG->menu();
                break;
            case 3:
                menuBP->menu();
                break;
            case 4:
                menuLDV->menu();
                break;
            case 0:
                cout << "Dang thoat chuong trinh va sao luu du lieu...\n";
                break;
            default: 
                cout << "Lua chon khong hop le! Vui long chon lai.\n";
                system("pause");
            }
        } while (choice != 0);
    }
};

#endif