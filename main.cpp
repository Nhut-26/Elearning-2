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
// xoa cac node cu 
void freeList(NV& head) {
    while (head) {
        NV tmp = head;
        head = head->next;
        delete tmp;
    }
}
// kiem tra co trung ma nhan vien hay k
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
        cout << "Nhap sai! Vui long nhap so ngay cong: ";
    }

    cout << "Nhap luong ngay: ";
    while (!(cin >> nv.luongNgay)) {
        cin.clear();
        cout << "Nhap sai! Vui long nhap luong ngay: ";
    }

    return nv;
}

void nhapDanhSach(NV& head, int n) {
    for (int i = 0; i < n; i++) {
        cout << "\n===== NHAN VIEN " << (i + 1) << " =====\n";
        NhanVien nv = nhap1NV(head);
        append(head, nv);
    }
}

// ====== Ghi file ======
void ghiFile(NV head, const string& tenFile) {
    ofstream fout(tenFile);
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
    cout << "\nDa ghi danh sach ra file: " << tenFile << "\n";
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

    freeList(head);

    string line;
    // bo header
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
        getline(ss, thucLinhStr); // co/khong deu duoc

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

        // neu file co maNV trung, bo qua (giu dung yeu cau unique)
        if (isDuplicateId(head, nv.maNV)) continue;

        append(head, nv);
    }

    fin.close();
    return true;
}

// ====== In danh sach ======
void inDanhSach(NV head) {
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
// ====== Tim theo ma nv =============
NV timTheoMa(NV head,const string& ma){

    for(NV p = head ; p != NULL ; p = p->next){
        if(p->data.maNV == ma) return p;
        cout<<"rrr";
    }
    return NULL;
    
}
// ======= In Danh sach theo ma nhan vien ========
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
// ======= Tim kiem theo ten ==============
string toLowerStr(string s){
    for(char &c : s){
        c = (char)tolower((unsigned char)c);
    }
            return s;
}

void timVaInTheoTen(NV head, const string &ten){
    string k = toLowerStr(ten);
    bool found = false;
    while(head != NULL){
        if(toLowerStr(head->data.hoTen) == k){
            inMaNV(head->data);
            found = true;
        }
        head = head->next;
    }
    if(!found) cout << "Khong tim thay!\n";
}
// ====== In Nhan Vien co luong thuc linh thap nhat ra man hinh ====== (su dung thuat toan tuyen tinh)
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

/*Sap xep theo selection sort*/

void sapXepThucLinh(NV head)
{
    if(head==NULL)
    {
        cout << "Danh sach rong \n";
        return;
    }
    for(NV p = head; p != NULL; p = p -> next)
    {
        for(NV q = p -> next; q != NULL; q = q -> next)
        {
            if(thucLinh(q->data) > thucLinh(p->data))
            {
                NhanVien temp = q->data;
                q->data = p->data;
                p->data = temp;

            }
        }
    }
}

// ====== MAIN demo: nhap -> ghi -> doc -> in ======
int main() {
    NV head = NULL;

   int lc = -1;

while (lc != 0) {
    cout << "--------------MENU--------------\n";
    cout << "1. Nhap danh sach Nhan Vien\n";
    cout << "2. In danh sach Nhan Vien\n";
    cout << "3. Doc file danh sach nhan vien\n";
    cout << "4. Tim thong tin nhan vien theo ma nhan vien\n";
    cout << "5. Tim nhan vien theo ten\n";
    cout << "6. In luong thuc linh thap nhat ra man hinh\n";
    cout << "7. Sap xep nhan vien giam dan theo thuc linh\n";
    cout << "0. Thoat\n";
    cout << "Nhap lua chon: ";
    cin >> lc;

    switch (lc) {
        case 1: {
            NhanVien nv = nhap1NV(head);
            append(head, nv);
            ghiFile(head, "DSNV.txt");
            break;
        }
        case 2:
            inDanhSach(head);
            break;
        case 3:
            docFile(head, "DSNV.txt");
            break;
        case 4: {
            string ma;
            cout << "Nhap ma nhan vien can tim: ";
            cin >> ma;
            NV p = timTheoMa(head, ma);
            if (p != NULL)
                inMaNV(p->data);
            else
                cout << "Khong tim thay nhan vien!\n";
            break;
        }
        case 5: {
            string ten;
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
            docFile(head, "DSNV_SAPXEP.txt");
            sapXepThucLinh(head);
            ghiFile(head, "DSNV_SAPXEP.txt");
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