#include <iostream>
#include <list>
#include <vector>
#include <limits>
#include <fstream>

using namespace std;
struct Date{
    int ngay;
    int thang;
    int nam;
};

struct NhanVien{
    string maNV;
    string hoTen;
    Date d;
    string email;
    string diachi;
    string sdt;
    int ngayCongLamViec;
    long long luongNgay;
};

long long thucLinh(const NhanVien &a){
    return 1LL* a.ngayCongLamViec * a.luongNgay;
}
//Danh sach tuyen tinh vi neu ds n sinh vien thi phai nhap n lan
bool kiemtraID(const vector<NhanVien>& ds, const string& id){
    for(const auto &nv : ds){
        if(nv.maNV == id)
        { 
            return true;
        }
    }
        return false;
}
// Ham them danh sach
void themDS(vector<NhanVien>& ds, NhanVien nv){
    while(kiemtraID(ds, nv.maNV)){
        cout << "Ma nhan vien da bi trung! Vui long nhap lai.\n";
        cin >> nv.maNV;
    }
    ds.push_back(nv);
}
// Ham nhap danh sach Nhan Vien
void nhapDS(vector<NhanVien>& ds,NhanVien nv,Date d){
    cout <<"Nhap ma so Nhan Vien : ";
    cin >> nv.maNV;
    cin.ignore();  
    cout <<"Ho Va Ten : ";
    getline(cin,nv.hoTen);
    cout <<"Nhap ngay/thang/nam Nhan Vien : ";
    cin >> d.ngay;
    cin >> d.thang;
    cin >> d.nam;
    nv.d = d;
    cout <<"Nhap email Nhan Vien : ";
    cin >> nv.email;
    cin.ignore();
    cout <<"Nhap dia chi cua Nhan Vien : ";
    getline(cin,nv.diachi);
    cout <<"Nhap so dien thoai cua Nhan Vien : "; 
    cin >> nv.sdt;
    cout <<"Nhap ngay cong lam viec : ";
    cin >> nv.ngayCongLamViec;
    cout << "Nhap luong ngay: ";
    cin >> nv.luongNgay;
    themDS(ds,nv);
}
// Ham ghi ra file
void ghiFile(const vector<NhanVien>& ds,const string& tenFile){
    ofstream fout(tenFile);
    if(!fout.is_open()){
        cout << "Khong mo duoc file " << tenFile << endl;
        return;
    }
    fout << "Ma_NV|HO_TEN|NGAY_SINH|EMAIL|DIA_CHI|SDT|NGAY_CONG|LUONG_NGAY|THUC_LINH\n";
    for(const auto& nv : ds){
        fout << nv.maNV << '|'
             << nv.hoTen << '|'
             << nv.d.ngay << '/' << nv.d.thang << '/' << nv.d.nam << '|'
             <<nv.email <<'|'
             <<nv.diachi <<'|'
             <<nv.sdt << '|'
             <<nv.ngayCongLamViec <<'|'
             <<nv.luongNgay << '|'
             << thucLinh(nv) << "\n";
    }
    fout.close();
    cout << "\nDa ghi " << ds.size() << " nhan vien ra file : " << tenFile << "\n";
}
int main(){
        vector<NhanVien> ds;
    int n;
    cout << "Nhap so luong nhan vien: ";
    cin >> n;
    for(int i = 0; i < n; i++){
        NhanVien nv;
        Date d;
        nhapDS(ds,nv,d);
    }
    ghiFile(ds, "DSNV.txt");

    return 0;
}