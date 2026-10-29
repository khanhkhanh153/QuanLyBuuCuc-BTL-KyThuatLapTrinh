//Nguyễn Mạnh Tú

#ifndef BUUCUC_H
#define BUUCUC_H

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

using namespace std;

// --- LOP THUC THE ---
class BuuCuc : public Entity
{
private:
    string tenBuuCuc;
    string diaChi;
    string soDienThoai;

public:
    BuuCuc() : Entity(), tenBuuCuc(""), diaChi(""), soDienThoai("") {}

    string getTenBuuCuc() const
    {
        return tenBuuCuc;
    }

    void setTenBuuCuc(const string& ten)
    {
        if (ten.empty())
        {
            throw invalid_argument("Ten buu cuc khong duoc de trong!");
        }
        tenBuuCuc = ten;
    }

    string getDiaChi() const
    {
        return diaChi;
    }

    void setDiaChi(const string& dc)
    {
        if (dc.empty())
        {
            throw invalid_argument("Dia chi khong duoc de trong!");
        }
        diaChi = dc;
    }

    string getSoDienThoai() const
    {
        return soDienThoai;
    }

    void setSoDienThoai(const string& sdt)
    {
        if (sdt.empty())
        {
            throw invalid_argument("So dien thoai khong duoc de trong!");
        }
        for (char c : sdt)
        {
            if (!isdigit(c))
            {
                throw invalid_argument("So dien thoai khong duoc chua chu cai hoac ky tu dac biet!");
            }
        }
        soDienThoai = sdt;
    }

    void nhap() override
    {
        string temp;
        while (true)
        {
            try
            {
                cout << "Nhap ten buu cuc: ";
                getline(cin, temp);
                setTenBuuCuc(temp);
                break;
            }
            catch (const exception& e)
            {
                cout << "Loi: " << e.what() << " Vui long nhap lai.\n";
            }
        }
        while (true)
        {
            try
            {
                cout << "Nhap dia chi: ";
                getline(cin, temp);
                setDiaChi(temp);
                break;
            }
            catch (const exception& e)
            {
                cout << "Loi: " << e.what() << " Vui long nhap lai.\n";
            }
        }
        while (true)
        {
            try
            {
                cout << "Nhap so dien thoai: ";
                getline(cin, temp);
                setSoDienThoai(temp);
                break;
            }
            catch (const exception& e)
            {
                cout << "Loi: " << e.what() << " Vui long nhap lai.\n";
            }
        }
    }

    void xuat() const override
    {
        cout << left << setw(10) << maID << "| " << setw(25) << tenBuuCuc
            << "| " << setw(30) << diaChi << "| " << setw(15) << soDienThoai << endl;
    }

    string toFileRecord() const override
    {
        stringstream ss;
        ss << maID << "|" << tenBuuCuc << "|" << diaChi << "|" << soDienThoai;
        return ss.str();
    }

    void fromFileRecord(const string& record) override
    {
        stringstream ss(record);
        string item;
        vector<string> tokens;
        while (getline(ss, item, '|'))
        {
            tokens.push_back(item);
        }
        if (tokens.size() == 4)
        {
            maID = tokens[0];
            tenBuuCuc = tokens[1];
            diaChi = tokens[2];
            soDienThoai = tokens[3];
        }
    }
};

// --- LOP GIAO DIEN (MENU) ---
class BuuCucMenu
{
private:
    Repository<BuuCuc>& repo;

    // Khai bao con tro ham (callback) kiem tra rang buoc chung
    function<bool(string)> kiemTraRangBuoc;

    void xoaManHinh() const
    {
        system("cls");
    }

    void tamDung() const
    {
        cout << "\nNhan Enter de tiep tuc...";
        cin.get();
    }

    void inTieuDeBang() const
    {
        cout << left << setw(10) << "Ma BC" << "| " << setw(25) << "Ten Buu Cuc"
            << "| " << setw(30) << "Dia Chi" << "| " << setw(15) << "So Dien Thoai" << endl;
        cout << string(85, '-') << endl;
    }

public:
    BuuCucMenu(Repository<BuuCuc>& r, function<bool(string)> kiemTra = nullptr)
        : repo(r), kiemTraRangBuoc(kiemTra)
    {
    }

    void themMoi()
    {
        cout << "\n=== THEM MOI BUU CUC ===\n";
        BuuCuc bc;
        string id;
        while (true)
        {
            try
            {
                cout << "Nhap ma buu cuc (VD: BC01): ";
                getline(cin, id);
                bc.setMaID(id);
                if (repo.findById(id) != nullptr)
                {
                    throw invalid_argument("Ma buu cuc da ton tai!");
                }
                break;
            }
            catch (const exception& e)
            {
                cout << "Loi: " << e.what() << " Vui long nhap lai.\n";
            }
        }
        bc.nhap();
        repo.add(bc);
        cout << "-> Them moi thanh cong!\n";
    }

    void xemDanhSach() const
    {
        auto list = repo.getAll();
        if (list.empty())
        {
            cout << "\nDanh sach trong!\n";
            return;
        }
        cout << "\n================================= DANH SACH BUU CUC =================================\n";
        inTieuDeBang();
        for (const auto& item : list)
        {
            item.xuat();
        }
        cout << "Tong so buu cuc: " << list.size() << endl;
    }

    void timKiem() const
    {
        string id;
        cout << "Nhap ma buu cuc can tim: ";
        getline(cin, id);
        BuuCuc* result = repo.findById(id);
        if (result != nullptr)
        {
            cout << "\nDa tim thay thong tin:\n";
            inTieuDeBang();
            result->xuat();
        }
        else
        {
            cout << "Khong tim thay buu cuc ma: " << id << "\n";
        }
    }

    void sapXep()
    {
        auto list = repo.getAll();
        if (list.empty())
        {
            cout << "\nDanh sach trong!\n";
            return;
        }

        sort(list.begin(), list.end(), [](const BuuCuc& a, const BuuCuc& b)
            {
                return a.getTenBuuCuc() < b.getTenBuuCuc();
            });

        cout << "-> Da sap xep danh sach hien thi theo Ten Buu Cuc (A-Z)!\n";
        cout << "\n============================= KET QUA SAP XEP =============================\n";
        inTieuDeBang();
        for (const auto& item : list)
        {
            item.xuat();
        }
    }

    void capNhat()
    {
        string id;
        cout << "Nhap ma buu cuc can cap nhat: ";
        getline(cin, id);
        BuuCuc* old = repo.findById(id);
        if (old == nullptr)
        {
            cout << "Khong tim thay ma nay!\n";
            return;
        }

        cout << "\nThong tin hien tai:\n";
        inTieuDeBang();
        old->xuat();
        cout << "\n(Luu y: De trong neu muon giu nguyen gia tri cu)\n";

        BuuCuc newData = *old;
        string temp;
        cout << "Nhap ten moi [" << old->getTenBuuCuc() << "]: ";
        getline(cin, temp);
        if (!temp.empty())
        {
            newData.setTenBuuCuc(temp);
        }

        cout << "Nhap dia chi moi [" << old->getDiaChi() << "]: ";
        getline(cin, temp);
        if (!temp.empty())
        {
            newData.setDiaChi(temp);
        }

        cout << "Nhap so dien thoai moi [" << old->getSoDienThoai() << "]: ";
        getline(cin, temp);
        if (!temp.empty())
        {
            newData.setSoDienThoai(temp);
        }

        if (repo.update(id, newData))
        {
            cout << "-> Cap nhat thanh cong!\n";
        }
        else
        {
            cout << "Loi khi cap nhat!\n";
        }
    }

    void xoa()
    {
        string id;
        cout << "Nhap ma buu cuc can xoa: ";
        getline(cin, id);
        BuuCuc* target = repo.findById(id);
        if (target == nullptr)
        {
            cout << "Khong tim thay ma nay!\n";
            return;
        }

        cout << "\nThong tin chuan bi xoa:\n";
        inTieuDeBang();
        target->xuat();

        // KIEM TRA RANG BUOC TRUOC KHI XOA
        if (kiemTraRangBuoc != nullptr)
        {
            bool dangDuocSuDung = kiemTraRangBuoc(id);
            if (dangDuocSuDung)
            {
                cout << "\nKHONG THE XOA!\n";
                cout << "-> Buu cuc nay hien dang duoc su dung trong Phieu Gui hoac Buu Pham.\n";
                cout << "-> Vui long kiem tra lai cac lien ket du lieu truoc khi xoa.\n";
                return; // Chan thao tac xoa
            }
        }

        cout << "\nBan co chac chan muon xoa khong? (y/n): ";
        char confirm;
        cin >> confirm;
        cin.ignore(10000, '\n');

        if (confirm == 'y' || confirm == 'Y')
        {
            if (repo.removeById(id))
            {
                cout << "-> Xoa thanh cong!\n";
            }
        }
        else
        {
            cout << "Huy thao tac xoa.\n";
        }
    }

    void menu()
    {
        int choice = -1;
        do
        {
            xoaManHinh();
            cout << "========== QUAN LY BUU CUC ==========\n";
            cout << "1. Them buu cuc moi\n";
            cout << "2. Xem danh sach\n";
            cout << "3. Tim kiem\n";
            cout << "4. Sap xep theo Ten (A-Z)\n";
            cout << "5. Cap nhat\n";
            cout << "6. Xoa\n";
            cout << "0. Quay lai menu chinh\n";
            cout << "=====================================\nLua chon cua ban: ";
            if (!(cin >> choice))
            {
                cin.clear();
                choice = -1;
            }
            cin.ignore(10000, '\n');
            if (choice == 0)
            {
                break;
            }

            xoaManHinh();
            switch (choice)
            {
            case 1:
                themMoi();
                break;
            case 2:
                xemDanhSach();
                break;
            case 3:
                timKiem();
                break;
            case 4:
                sapXep();
                break;
            case 5:
                capNhat();
                break;
            case 6:
                xoa();
                break;
            default:
                cout << "Lua chon khong hop le!\n";
            }
            tamDung();
        } while (choice != 0);
    }
};

#endif
