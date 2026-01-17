#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <limits>
#include <cctype>

using namespace std;

struct Date {
    int ngay, thang, nam;
};

struct NhanVien {
    string maNV, hoTen, email, diachi, sdt;
    Date d;
    int ngayCongLamViec;
    long long luongNgay;
};

long long thucLinh(const NhanVien& a) {
    return 1LL * a.ngayCongLamViec * a.luongNgay;
}

// ====== DSLK Don ======
struct Node {
    NhanVien data;
    Node* next;
};
typedef struct Node* NV;

NV createNode(const NhanVien& nv) {
    NV p = new Node;
    p->data = nv;
    p->next = NULL;
    return p;
}

// Xoa cac node cu 
void freeList(NV& head) {
    while (head) {
        NV tmp = head;
        head = head->next;
        delete tmp;
    }
}

// Kiem tra co trung ma nhan vien hay khong
bool isDuplicateId(NV head, const string& id) {
    for (NV p = head; p != NULL; p = p->next) {
        if (p->data.maNV == id) return true;
    }
    return false;
}

void append(NV& head, const NhanVien& nv) {
    NV p = createNode(nv);
    if (head == NULL) {
        head = p;
        return;
    }
    NV tail = head;
    while (tail->next != NULL) tail = tail->next;
    tail->next = p;
}

// ====== Nhap ======
NhanVien nhap1NV(NV head) {
    NhanVien nv;

    // maNV (unique)
    while (true) {
        cout << "Nhap ma so Nhan Vien: ";
        if (!(cin >> nv.maNV)) {
            cin.clear();
            continue;
        }
        if (!isDuplicateId(head, nv.maNV)) break;
        cout << "Ma nhan vien bi trung! Nhap lai.\n";
    }

    cin.ignore();
    cout << "Ho va Ten: ";
    getline(cin, nv.hoTen);

    cout << "Nhap ngay/thang/nam sinh (vd: 1 12 2005): ";
    while (!(cin >> nv.d.ngay >> nv.d.thang >> nv.d.nam)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Xoa bo dem
        cout << "Nhap sai! Vui long nhap lai (vd: 1 12 2005): ";
    }

    cout << "Nhap email: ";
    cin >> nv.email;

    cin.ignore();
    cout << "Nhap dia chi: ";
    getline(cin, nv.diachi);

    cout << "Nhap so dien thoai: ";
    cin >> nv.sdt;

    cout << "Nhap ngay cong lam viec: ";
    while (!(cin >> nv.ngayCongLamViec)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Nhap sai! Vui long nhap so ngay cong: ";
    }

    cout << "Nhap luong ngay: ";
    while (!(cin >> nv.luongNgay)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Nhap sai! Vui long nhap luong ngay: ";
    }

    return nv;
}

// ====== Ghi file (Ghi de - Dung cho ham Xoa/Sap xep) ======
void ghiFile(NV head, const string& tenFile) {
    ofstream fout(tenFile); // Mac dinh la ios::out (ghi de)
    if (!fout.is_open()) {
        cout << "Khong mo duoc file " << tenFile << "\n";
        return;
    }

    fout << "Ma_NV|HO_TEN|NGAY_SINH|EMAIL|DIA_CHI|SDT|NGAY_CONG|LUONG_NGAY|THUC_LINH\n";
    for (NV p = head; p != NULL; p = p->next) {
        const NhanVien& nv = p->data;
        fout << nv.maNV << '|'
             << nv.hoTen << '|'
             << nv.d.ngay << '/' << nv.d.thang << '/' << nv.d.nam << '|'
             << nv.email << '|'
             << nv.diachi << '|'
             << nv.sdt << '|'
             << nv.ngayCongLamViec << '|'
             << nv.luongNgay << '|'
             << thucLinh(nv) << "\n";
    }

    fout.close();
    cout << "\nDa ghi lai toan bo danh sach ra file: " << tenFile << "\n";
}

// ====== Ham moi: Ghi them vao cuoi file (Dung cho ham Nhap) ======
void ghiThemVaoCuoiFile(const NhanVien& nv, const string& tenFile) {
    // ios::app giup ghi noi tiep vao cuoi file
    ofstream fout(tenFile, ios::app); 
    if (!fout.is_open()) {
        cout << "Khong mo duoc file de ghi them!\n";
        return;
    }
    
    // Kiem tra neu file rong thi ghi header truoc 
    fout.seekp(0, ios::end);
    if (fout.tellp() == 0) {
        fout << "Ma_NV|HO_TEN|NGAY_SINH|EMAIL|DIA_CHI|SDT|NGAY_CONG|LUONG_NGAY|THUC_LINH\n";
    }

    fout << nv.maNV << '|'
         << nv.hoTen << '|'
         << nv.d.ngay << '/' << nv.d.thang << '/' << nv.d.nam << '|'
         << nv.email << '|'
         << nv.diachi << '|'
         << nv.sdt << '|'
         << nv.ngayCongLamViec << '|'
         << nv.luongNgay << '|'
         << thucLinh(nv) << "\n";

    fout.close();
}

// ====== Doc file ======
bool tachNgaySinh(const string& ns, Date& d) {
    char c1, c2;
    stringstream ss(ns);
    return (ss >> d.ngay >> c1 >> d.thang >> c2 >> d.nam && c1 == '/' && c2 == '/');
}

bool docFile(NV& head, const string& tenFile) {
    ifstream fin(tenFile);
    if (!fin.is_open()) {
        cout << "Khong mo duoc file " << tenFile << "\n";
        return false;
    }

    freeList(head); // Xoa danh sach cu trong RAM truoc khi doc moi

    string line;
    // Bo qua dong header
    if (!getline(fin, line)) {
        fin.close();
        return false;
    }

    while (getline(fin, line)) {
        if (line.empty()) continue;

        string maNV, hoTen, ngaySinhStr, email, diachi, sdt;
        string ngayCongStr, luongNgayStr, thucLinhStr;

        stringstream ss(line);

        if (!getline(ss, maNV, '|')) continue;
        if (!getline(ss, hoTen, '|')) continue;
        if (!getline(ss, ngaySinhStr, '|')) continue;
        if (!getline(ss, email, '|')) continue;
        if (!getline(ss, diachi, '|')) continue;
        if (!getline(ss, sdt, '|')) continue;
        if (!getline(ss, ngayCongStr, '|')) continue;
        if (!getline(ss, luongNgayStr, '|')) continue;
        getline(ss, thucLinhStr);

        NhanVien nv;
        nv.maNV = maNV;
        nv.hoTen = hoTen;
        nv.email = email;
        nv.diachi = diachi;
        nv.sdt = sdt;

        if (!tachNgaySinh(ngaySinhStr, nv.d)) continue;

        try {
            nv.ngayCongLamViec = stoi(ngayCongStr);
            nv.luongNgay = stoll(luongNgayStr);
        } catch (...) {
            continue;
        }

        // tranh trung lap neu file loi
        if (isDuplicateId(head, nv.maNV)) continue;

        append(head, nv);
    }

    fin.close();
    cout << "Da doc du lieu tu file " << tenFile << " len chuong trinh.\n";
    return true;
}

// ====== In danh sach ======
void inDanhSach(NV head) {
    if (head == NULL) {
        cout << "\nDanh sach trong!\n";
        return;
    }
    cout << "\n=========== DANH SACH NHAN VIEN ===========\n";
    for (NV p = head; p != NULL; p = p->next) {
        const NhanVien& nv = p->data;
        cout << "MaNV: " << nv.maNV
             << " | HoTen: " << nv.hoTen
             << " | NS: " << nv.d.ngay << "/" << nv.d.thang << "/" << nv.d.nam
             << " | Email: " << nv.email
             << " | DiaChi: " << nv.diachi
             << " | SDT: " << nv.sdt
             << " | NgayCong: " << nv.ngayCongLamViec
             << " | LuongNgay: " << nv.luongNgay
             << " | ThucLinh: " << thucLinh(nv)
             << "\n";
    }
}

// ======= In 1 Nhan Vien ========
void inMaNV(const NhanVien& nv){
    cout << "MaNV: " << nv.maNV
         << " | HoTen: " << nv.hoTen
         << " | NS: " << nv.d.ngay << "/" << nv.d.thang << "/" << nv.d.nam
         << " | Email: " << nv.email
         << " | DiaChi: " << nv.diachi
         << " | SDT: " << nv.sdt
         << " | NgayCong: " << nv.ngayCongLamViec
         << " | LuongNgay: " << nv.luongNgay
         << " | ThucLinh: " << thucLinh(nv)
         << "\n";
}

// ======= Helper xu ly chuoi ========
string toLowerStr(string s){
    for(char &c : s){
        c = (char)tolower((unsigned char)c);
    }
    return s;
}

// ======= Tim kiem theo ten ==============
void timVaInTheoTen(NV head, const string &ten){
    string k = toLowerStr(ten);
    bool found = false;
    while(head != NULL){
        if(toLowerStr(head->data.hoTen).find(k) != string::npos){ // Tim gan dung
            inMaNV(head->data);
            found = true;
        }
        head = head->next;
    }
    if(!found) cout << "Khong tim thay!\n";
}

// ======= Tim kiem theo ma ==============
void timVaInTheoMa(NV head, const string &ma){
    bool found = false;
    while(head != NULL){
        if(head->data.maNV == ma){
            inMaNV(head->data);
            found = true;
            break;
        }
        head = head->next;
    }
    if(!found) cout << "Khong tim thay!\n";
}

// ====== In Nhan Vien thuc linh thap nhat ======
void thucLinhThapNhat(NV head){
    if(head == NULL){
        cout << "Danh Sach rong \n";
        return;
    }
    long long minluong = thucLinh(head->data);
    for(NV p = head; p != NULL ; p = p->next){
        long long tl = thucLinh(p->data);
        if(tl < minluong) minluong = tl;
    }
    cout <<"Thuc linh thap nhat: " << minluong << endl;
    for(NV p = head; p != NULL ; p = p->next){
        if(thucLinh(p->data) == minluong){
            inMaNV(p->data);
        }
    }
}

// ====== Sap xep ======
void sapXepThucLinh(NV head)
{
    if(head==NULL) {
        cout << "Danh sach rong \n";
        return;
    }
    for(NV p = head; p != NULL; p = p -> next) {
        for(NV q = p -> next; q != NULL; q = q -> next) {
            if(thucLinh(q->data) > thucLinh(p->data)) {
                NhanVien temp = q->data;
                q->data = p->data;
                p->data = temp;
            }
        }
    }
    cout << "Da sap xep danh sach giam dan theo thuc linh.\n";
}

// ====== Xoa Nhan Vien  ======
void xoaNhanVien(NV& head, const string& ma)
{
    if (head == NULL) return;

    // Truong hop node can xoa o dau
    if (head->data.maNV == ma) {
        NV temp = head;
        head = head->next;
        delete temp;
        cout << "Da xoa nhan vien co ma: " << ma << endl;
        return;
    }

    // Truong hop node o giua hoac cuoi
    NV p = head;
    while (p->next != NULL) {
        if (p->next->data.maNV == ma) {
            NV temp = p->next;
            p->next = p->next->next;
            delete temp;
            cout << "Da xoa nhan vien co ma: " << ma << endl;
            return;
        }
        p = p->next;
    }
    cout << "Khong tim thay ma nhan vien can xoa!\n";
}

// ====== MAIN ======
int main() {
    NV head = NULL;
    string ma;
    string ten;
    int lc = -1;

    // Tu dong load du lieu cu khi mo chuong trinh (optional)
    docFile(head, "DSNV.txt");

    while (lc != 0) {
        cout << "\n--------------MENU--------------\n";
        cout << "1. Nhap danh sach Nhan Vien (Them moi)\n";
        cout << "2. In danh sach Nhan Vien ra man hinh\n";
        cout << "3. Doc lai file danh sach nhan vien (Reset)\n";
        cout << "4. Tim thong tin nhan vien theo ma\n";
        cout << "5. Tim nhan vien theo ten\n";
        cout << "6. In luong thuc linh thap nhat\n";
        cout << "7. Sap xep giam dan theo thuc linh & Ghi file\n";
        cout << "8. Xoa nhan vien theo ma & Ghi file\n";
        cout << "0. Thoat\n";
        cout << "Nhap lua chon: ";
        cin >> lc;

        switch (lc) {
            case 1: {
                int n;
                cout << "Nhap so luong nhan vien muon them: ";
                cin >> n;
                // Dung vong lap for de nhap va ghi noi tiep
                for (int i = 0; i < n; i++) {
                    cout << "\n--- Nhap thong tin nhan vien thu " << i + 1 << " ---\n";
                    NhanVien nv = nhap1NV(head); // Nhap va check trung ma voi list hien tai
                    
                    append(head, nv); // Them vao bo nho chuong trinh
                   
                    ghiThemVaoCuoiFile(nv, "DSNV.txt"); 
                }
                cout << "\nDa them va ghi " << n << " nhan vien vao file DSNV.txt thanh cong!\n";
                break;
            }
            case 2:
                inDanhSach(head);
                break;
            case 3:
                docFile(head, "DSNV.txt");
                break;
            case 4: {
                cout << "Nhap ma nhan vien can tim: ";
                cin >> ma;
                timVaInTheoMa(head, ma);
                break;
            }
            case 5: {
                cout << "Nhap ten nhan vien can tim: ";
                cin.ignore();
                getline(cin, ten);
                timVaInTheoTen(head, ten);
                break;
            }
            case 6:
                thucLinhThapNhat(head);
                break;
            case 7:
                sapXepThucLinh(head);
                ghiFile(head, "DSNV_SAPXEP.txt"); // Ghi file rieng cho sap xep
                break;
            case 8:     
                cout << "Nhap ma nhan vien can xoa: ";
                cin >> ma;
                xoaNhanVien(head, ma);
                ghiFile(head, "DSNV.txt"); // Xoa xong phai ghi de lai toan bo file
                break;
            case 0:
                cout << "Thoat chuong trinh!\n";
                break;
            default:
                cout << "Cu phap khong hop le!\n";
        }
    }

    freeList(head);
    return 0;
}