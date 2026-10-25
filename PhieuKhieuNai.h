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
