#include <ctime>
#include <fstream>
#include <iostream>
#include <string>
#include <regex>
#include <unordered_map>
#include <queue>
#include <ctime>
#include <conio.h>
#include <windows.h>
#include "Json.hpp"

#ifndef NDEBUG
#define NDEBUG false
#else
#define NDEBUG true
#endif

#ifndef DEBUG
#define DEBUG !NDEBUG
#endif

using namespace std;
using json = nlohmann::json;

string LTrim(const string &s) {
    return regex_replace(s, regex("^\\s+"), string(""));
}

string RTrim(const string &s) {
    return regex_replace(s, regex("\\s+$"), string(""));
}

string Trim(const string &s) {
    return LTrim(RTrim(s));
}

string TimeToString(time_t time) {
    tm tmp;
    localtime_s(&tmp, &time);
    char out[51];
    strftime(out, 50, "%F %T", &tmp);
    return out;
}

// 1. Thông tin phiếu giải quyết công việc
struct Ticket
{
    string ID;
    string StudentName;
    string ClassName;
    string Content;
    time_t CreatedAt{};
    time_t UpdatedAt{};
    bool Priority;

    Ticket(string id, string studentName, string className, string content) {
        ID = Trim(id);
        StudentName = Trim(studentName);
        ClassName = Trim(className);
        Content = Trim(content);
        CreatedAt = time(0);
        UpdatedAt = CreatedAt;
        Priority = false;
    }
    Ticket(string id, string studentName, string className, string content, bool priority) {
        ID = Trim(id);
        StudentName = Trim(studentName);
        ClassName = Trim(className);
        Content = Trim(content);
        CreatedAt = time(0);
        UpdatedAt = CreatedAt;
        Priority = priority;
    }
    Ticket() {
        UpdatedAt = CreatedAt;
        Priority = 0;
    }
};

struct CompareTicket
{
    bool operator()(Ticket &t1, Ticket &t2) {
        if (t1.Priority == true || t2.Priority == true) {
            if (t1.Priority == true && t2.Priority == true) {
                t2.Priority > t1.Priority;
                return t1.CreatedAt > t2.CreatedAt;
            }
            return t2.Priority > t1.Priority;
        }
        return t1.CreatedAt > t2.CreatedAt;
    }
};

Ticket NhapPhieu() {
    Ticket t = Ticket();
    cout << "Ma so: "; cin.ignore(); getline(cin, t.ID);
    t.ID = Trim(t.ID);
    cout << "Ten sinh vien: "; getline(cin, t.StudentName);
    t.StudentName = Trim(t.StudentName);
    cout << "Lop: "; getline(cin, t.ClassName);
    t.ClassName = Trim(t.ClassName);
    cout << "Noi dung: "; getline(cin, t.Content);
    t.Content = Trim(t.Content);
    t.CreatedAt = time(0);
    t.UpdatedAt = t.CreatedAt;
    return t;
}

void InPhieu(Ticket t) {
    cout << "Ma so: " << t.ID << endl;
    cout << "Ten sinh vien: " << t.StudentName << endl;
    cout << "Lop: " << t.ClassName << endl;
    cout << "Noi dung: " << t.Content << endl;
    cout << "Uu tien: " << t.Priority << endl;
    cout << "Ngay tao phieu: " << TimeToString(t.CreatedAt) << endl;
    cout << "Ngay cap nhat phieu: " << TimeToString(t.UpdatedAt) << endl;
}

// 2. Tạo danh sách hàng đợi được nhập từ bàn phím
void NhapDanhSach(priority_queue<Ticket, vector<Ticket>, CompareTicket> &pq) {
    size_t n;
    cout << "Nhap so luong phieu can nhap: ";
    cin >> n;
    cout << endl;
    for (size_t i = 0; i < n; i++) {
        pq.push(NhapPhieu());
        cout << endl;
    }
}

// 6. In dánh sách tất cả các phiếu trong hàng đợi
void InDanhSach(priority_queue<Ticket, vector<Ticket>, CompareTicket> pq) {
    while (!pq.empty())
    {
        InPhieu(pq.top());
        pq.pop();
        cout << endl;
    }
}

// Nhập dữ liệu mẫu vào hàng đợi
void DuLieuMau(priority_queue<Ticket, vector<Ticket>, CompareTicket> &pq) {
    pq.push(Ticket("214", "Nguyen Thi Kieu T", "T", "UTC2"));
    Sleep(1000);
    pq.push(Ticket("321", "Vo Van D", "D", "NEU"));
    Sleep(1000);
    pq.push(Ticket("938", "Vu Thi T", "T", "USSH", true));
    Sleep(1000);
    pq.push(Ticket("128", "Le Ngoc C", "C", "HUFI"));
    Sleep(1000);
    pq.push(Ticket("481", "Pham Bui Quoc K", "K", "HUB", true));
    Sleep(1000);
    pq.push(Ticket("531", "Nguyen Van E", "E", "HCMUTE", true));
    Sleep(1000);
    pq.push(Ticket("782", "Phan Phuoc V", "V", "SGU"));
    Sleep(1000);
    pq.push(Ticket("271", "Luu Tan L", "L", "OU", true));
    Sleep(1000);
    pq.push(Ticket("192", "Le Truong H", "H", "IUH"));
    Sleep(1000);
    pq.push(Ticket("621", "Hoang Minh P", "P", "HUTECH", true));
    Sleep(1000);
    pq.push(Ticket("194", "Nguyen Quoc K", "K", "UEH"));
    Sleep(1000);
    pq.push(Ticket("123", "Nguyen Huy B", "B", "HCMUT"));
    Sleep(1000);
    pq.push(Ticket("123", "Nguyen Duong Le C", "C", "HUFI"));
}

// 3. Thêm một phiếu giải quyết công việc vào hàng đợi
void ThemPhieu(priority_queue<Ticket, vector<Ticket>, CompareTicket> &pq, Ticket t) {
    pq.push(t);
}

// 4. Xóa một phần tử ra khỏi hàng đợi (Xóa thông qua ID)
bool XoaPhieu(priority_queue<Ticket, vector<Ticket>, CompareTicket> &pq, string id) {
    if (!pq.empty()) {
        bool _b = false;
        id = Trim(id);
        priority_queue<Ticket, vector<Ticket>, CompareTicket> out;
        while (!pq.empty()) {
            Ticket t = pq.top();
            pq.pop();
            if (t.ID != id) {
                out.push(t);
            }
            else {
                _b = true;
            }
        }
        pq = out;
        return _b;
    }
    else {
        return false;
    }
}

// 4. Xóa một phần tử ra khỏi hàng đợi (Nếu isTop là true thì xóa phần tử đầu tiên, ngược lại thì xóa thông qua ID)
bool XoaPhieu(priority_queue<Ticket, vector<Ticket>, CompareTicket> &pq, bool isTop) {
    if (!pq.empty()) {
        if (isTop) {
            pq.pop();
            return true;
        }
        else {
            string id;
            cout << "Nhap ma so can xoa khoi hang doi: ";
            cin.ignore();
            getline(cin, id);
            return XoaPhieu(pq, id);
        }
    }
    else {
        return false;
    }
}

// 2. Đọc danh sách hàng đợi từ file
bool DocDuLieuTuFile(string fileName, priority_queue<Ticket, vector<Ticket>, CompareTicket> &pq) {
    bool _b = false;
    try {
        ifstream file(fileName);
        json data;
        file >> data;
        for (json elem : data) {
            Ticket t;
            t.ID = elem["ID"].get<string>();
            t.StudentName = elem["StudentName"].get<string>();
            t.ClassName = elem["ClassName"].get<string>();
            t.Content = elem["Content"].get<string>();
            t.Priority = elem["Priority"].get<bool>();
            t.CreatedAt = (time_t)elem["CreatedAt"].get<int64_t>();
            t.UpdatedAt = (time_t)elem["UpdatedAt"].get<int64_t>();
            pq.push(t);
        }
        file.close();
        _b = true;
    }
    catch (...) {
        _b = false;
    }
    return _b;
}

// 2. Ghi danh sách hàng đợi vào file
bool GhiDuLieuVaoFile(string fileName, priority_queue<Ticket, vector<Ticket>, CompareTicket> pq) {
    bool _b = false;
    try {
        ofstream file(fileName);
        json data;
        while (!pq.empty()) {
            Ticket t = pq.top();
            pq.pop();
            data.push_back({
                {"ID", t.ID},
                {"StudentName", t.StudentName},
                {"ClassName", t.ClassName},
                {"Content", t.Content},
                {"Priority", t.Priority},
                {"CreatedAt", t.CreatedAt},
                {"UpdatedAt", t.UpdatedAt}
                });
        }
        file << setw(4) << data << endl;
        file.close();
        _b = true;
    }
    catch (...) {
        _b = false;
    }
    return _b;
}

// 5. Tìm kiếm một phiếu xử lý trong hàng đợi thông qua ID
bool TimPhieu(priority_queue<Ticket, vector<Ticket>, CompareTicket> pq, Ticket &t, string id) {
    id = Trim(id);
    while (!pq.empty()) {
        Ticket tmp = pq.top();
        pq.pop();
        if (Trim(id) == tmp.ID) {
            t = tmp;
            return true;
        }
    }
    return false;
}

// 7. Cập nhật thông tin của một phiếu xử lý trong hàng đợi (Được nhập từ bàn phím)
bool CapNhatPhieu(priority_queue<Ticket, vector<Ticket>, CompareTicket> &pq) {
    if (!pq.empty()) {
        bool _b = false;
        string id;
        cout << "Nhap ma so can cap nhat: "; cin.ignore(); getline(cin, id);
        id = Trim(id);
        priority_queue<Ticket, vector<Ticket>, CompareTicket> out;
        while (!pq.empty()) {
            Ticket t = pq.top();
            pq.pop();
            if (id == t.ID) {
                cout << "Ten sinh vien: "; getline(cin, t.StudentName);
                t.StudentName = Trim(t.StudentName);
                cout << "Lop: "; getline(cin, t.ClassName);
                t.ClassName = Trim(t.ClassName);
                cout << "Noi dung: "; getline(cin, t.Content);
                t.Content = Trim(t.Content);
                t.CreatedAt = t.CreatedAt;
                t.UpdatedAt = time(0);
                t.Priority = t.Priority;
                out.push(t);
                _b = true;
            }
            else {
                out.push(t);
            }
        }
        pq = out;
        return _b;
    }
    else {
        return false;
    }
}

// 7. Cập nhật thông tin của một phiếu xử lý trong hàng đợi
bool CapNhatPhieu(priority_queue<Ticket, vector<Ticket>, CompareTicket> &pq, Ticket t) {
    if (!pq.empty()) {
        bool _b = false;
        priority_queue<Ticket, vector<Ticket>, CompareTicket> out;
        while (!pq.empty()) {
            Ticket tmp = pq.top();
            pq.pop();
            if (t.ID == tmp.ID) {
                out.push(t);
                _b = true;
            }
            else {
                out.push(tmp);
            }
        }
        pq = out;
        return _b;
    }
    else {
        return false;
    }
}

// 10. Danh sách của việc sắp được xử lý (count là số lượng của việc sắp được xử lý)
priority_queue<Ticket, vector<Ticket>, CompareTicket> ViecSapDuocXuLy(priority_queue<Ticket, vector<Ticket>, CompareTicket> pq, size_t count) {
    priority_queue<Ticket, vector<Ticket>, CompareTicket> out;
    if (count > 0) {
        while (!pq.empty())
        {
            out.push(pq.top());
            pq.pop();
            count--;
            if (count == 0) {
                break;
            }
        }
        return out;
    }
    else {
        return out;
    }
}

// 9. Sinh viên lớp nào có nhiều việc cần khoa xử lý nhất
vector<string> LopCoNhieuViecCanKhoaXuLy(priority_queue<Ticket, vector<Ticket>, CompareTicket> pq) {
    unordered_map<string, size_t> map;
    while (!pq.empty())
    {
        Ticket t = pq.top();
        map[t.ClassName]++;
        pq.pop();
    }
    size_t maxCount = 0;
    for (auto &element : map) {
        if (element.second > maxCount) {
            maxCount = element.second;
        }
    }
    vector<string> className;
    for (auto& element : map) {
        if (element.second == maxCount) {
            className.push_back(element.first);
        }
    }
    return className;
}

// 8. Ưu tiên đưa một phiếu xử lý lên đầu danh sách (Ưu tiên thông qua ID)
bool PhieuUuTien(priority_queue<Ticket, vector<Ticket>, CompareTicket> &pq, string id) {
    if (!pq.empty()) {
        bool _b = false;
        id = Trim(id);
        priority_queue<Ticket, vector<Ticket>, CompareTicket> out;
        while (!pq.empty()) {
            Ticket t = pq.top();
            pq.pop();
            if (id == t.ID) {
                t.UpdatedAt = time(0);
                t.Priority = true;
                out.push(t);
                cout << "Phieu cua ma so " << id << " da duoc chuyen thanh phieu uu tien!" << endl;
                _b = true;
            }
            else {
                out.push(t);
            }
        }
        pq = out;
        return _b;
    }
    else {
        return false;
    }
}

// 8. Ưu tiên đưa một phiếu xử lý lên đầu danh sách (Được nhập từ bàn phím)
bool PhieuUuTien(priority_queue<Ticket, vector<Ticket>, CompareTicket> &pq) {
    if (!pq.empty()) {
        string id;
        cout << "Nhap ma so can uu tien: "; cin.ignore(); getline(cin, id);
        return PhieuUuTien(pq, id);
    }
    else {
        return false;
    }
}

int main()
{
    bool _break = false;
    priority_queue<Ticket, vector<Ticket>, CompareTicket> pq;
    while (!_break) {
        system("cls");
        size_t n = 0;
        if (DEBUG) {
            cout << "Build: DEBUG" << endl;
            cout << endl;
        }
        cout << "Bang dieu khien:" << endl;
        cout << "1.\tNhap danh sach cac phieu giai quyet cong viec vao hang doi" << endl;
        cout << "2.\tThem 1 phieu giai quyet cong viec vao hang doi" << endl;
        cout << "3.\tXoa 1 phieu giai quyet cong viec ra khoi hang doi" << endl;
        cout << "4.\tTim kiem 1 phieu xu ly trong hang doi" << endl;
        cout << "5.\tIn danh sach tat ca cac phieu trong hang doi" << endl;
        cout << "6.\tCap nhat thong tin cua 1 phieu xu ly trong hang doi" << endl;
        cout << "7.\tUu tien dua 1 phieu xu ly len dau danh sach" << endl;
        cout << "8.\tLop co nhieu viec can khoa xu ly nhat" << endl;
        cout << "9.\tDanh sach 10 viec sap duoc xu ly" << endl;
        cout << "10.\tDoc du lieu tu file" << endl;
        cout << "11.\tGhi du lieu vao file" << endl;
        cout << "12.\tThoat ung dung" << endl;
        if (DEBUG) {
            cout << "13.\tNhap du lieu mau vao hang doi (13 nguoi)" << endl;
        }
        cout << endl;
        cout << "Nhap lua chon: ";
        cin >> n;
        if (cin.fail()) {
            cin.clear();
            cin.ignore();
            continue;
        }
        cout << endl;

        switch (n)
        {
        case 1:
            NhapDanhSach(pq);
            break;
        case 2:
            ThemPhieu(pq, NhapPhieu());
            break;
        case 3: 
        {
            if (!XoaPhieu(pq, false)) {
                cout << "Hang doi rong hoac phieu can xoa khong ton tai!" << endl;
            }
            break;
        }
        case 4:
        {
            Ticket ticket;
            string id;
            cout << "Nhap ma so can tim: ";
            cin >> id;
            if (TimPhieu(pq, ticket, id)) {
                InPhieu(ticket);
            }
            else {
                cout << "Phieu can tim khong ton tai!" << endl;
            }
            break;
        }
        case 5:
            InDanhSach(pq);
            break;
        case 6:
        {
            if (!CapNhatPhieu(pq)) {
                cout << "Phieu can cap nhat khong ton tai!" << endl;
            }
            break;
        }
        case 7:
        {
            if (!PhieuUuTien(pq)) {
                cout << "Ma so can duoc uu tien khong ton tai!" << endl;
            }
            break;
        }
        case 8:
        {
            string out;
            for (auto x : LopCoNhieuViecCanKhoaXuLy(pq)) {
                out += " "+x;
            }
            cout << "Lop:" << out << endl;
            break;
        }
        case 9:
            InDanhSach(ViecSapDuocXuLy(pq, 10));
            break;
        case 10:
        {
            if (!DocDuLieuTuFile("data.txt", pq)) {
                cout << "File khong ton tai hoac du lieu bi hong!" << endl;
            }
            break;
        }
        case 11:
            GhiDuLieuVaoFile("data.txt", pq);
            break;
        case 12:
            _break = true;
            break;
        default:
            if (DEBUG) {
                switch (n)
                {
                case 13:
                    DuLieuMau(pq);
                    cout << "Da nhap du lieu mau vo hang doi thang cong!" << endl;
                    break;
                default:
                    continue;
                }
                cout << "Nhap phim bat ki de tiep tuc!" << endl;
                ignore = _getch();
            }
            continue;
        }
        cout << "Nhap phim bat ki de tiep tuc!" << endl;
        ignore = _getch();
    }
}
