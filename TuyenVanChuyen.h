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
        function<bool(string)> checkThamChieu = nullptr)
        : repoKH(repository), kiemTraDangDuocThamChieu(checkThamChieu) {}

    void themMoi() {
        cout << "\n=== THEM MOI KHACH HANG GUI ===\n";
        KhachHangGui kh;
        kh.nhap();
        try {
            repoKH.add(kh);
            cout << "-> Them khach hang thanh cong!\n";
        } catch (const exception& e) {
            cout << "-> Khong the them khach hang: " << e.what() << '\n';
        }
    }

    void xemDanhSach() const {
        const auto danhSach = repoKH.getAll();
        if (danhSach.empty()) {
            cout << "\nDanh sach khach hang gui trong!\n";
            return;
        }
        cout << "\n========================= DANH SACH KHACH HANG GUI =========================\n";
        inTieuDeBang();
        for (const auto& kh : danhSach) kh.xuat();
        cout << "Tong so khach hang: " << danhSach.size() << '\n';
    }

    void timKiem() {
        string ma;
        cout << "Nhap ma khach hang can tim: ";
        getline(cin >> ws, ma);
        KhachHangGui* kh = repoKH.findById(ma);
        if (kh) {
            cout << "\nDa tim thay khach hang:\n";
            inTieuDeBang();
            kh->xuat();
        } else {
            cout << "Khong tim thay khach hang co ma: " << ma << '\n';
        }
    }

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
        cout << "Nhap thong tin moi cho khach hang (ma KH giu nguyen: " << ma << ")\n";

        KhachHangGui moi;
        moi.nhap();
        moi.setMaID(ma);
        if (repoKH.update(ma, moi)) cout << "-> Cap nhat khach hang thanh cong!\n";
        else cout << "-> Cap nhat that bai!\n";
    }

    void xoa() {
        string ma;
        cout << "Nhap ma khach hang can xoa: ";
        getline(cin >> ws, ma);
        KhachHangGui* kh = repoKH.findById(ma);
        if (kh == nullptr) {
            cout << "Khong tim thay ma khach hang nay!\n";
            return;
        }

        // Rang buoc tham chieu duoc truyen tu Application.
        // Callback can kiem tra ca Phieu Gui va Phieu Khieu Nai.
        if (kiemTraDangDuocThamChieu && kiemTraDangDuocThamChieu(ma)) {
            cout << "-> KHONG THE XOA! Khach hang ma [" << ma
                 << "] dang duoc tham chieu trong Phieu Gui hoac Phieu Khieu Nai.\n";
            return;
        }

        cout << "\nThong tin chuan bi xoa:\n";
        inTieuDeBang();
        kh->xuat();
        cout << "Ban co chac chan muon xoa khong? (y/n): ";
        char xacNhan;
        cin >> xacNhan;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (xacNhan == 'y' || xacNhan == 'Y') {
            if (repoKH.removeById(ma)) cout << "-> Xoa khach hang thanh cong!\n";
            else cout << "-> Xoa khach hang that bai!\n";
        } else {
            cout << "Huy thao tac xoa.\n";
        }
    }

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
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Lua chon khong hop le!\n";
                continue;
            }
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            if (choice == 0) break;

            switch (choice) {
                case 1: themMoi(); break;
                case 2: xemDanhSach(); break;
                case 3: timKiem(); break;
                case 4: capNhat(); break;
                case 5: xoa(); break;
                default: cout << "Vui long chon tu 0 den 5!\n";
            }
            tamDung();
        } while (choice != 0);
    }
};

#endif
