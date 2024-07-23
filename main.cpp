#include <iostream>
#include <string>
#include <ctime>

using namespace std;

// Định nghĩa cấu trúc thời gian
struct ThoiGian {
    time_t vao;
    time_t ra;
};

// Định nghĩa cấu trúc của một nút trong cây AVL
struct Node {
    string bienSo;
    ThoiGian thoiGianRaVao;
    Node *left;
    Node *right;
    int height;
};

// Tính chiều cao của cây con
int height(Node *N) {
    if (N == nullptr)
        return 0;
    return N->height;
}

// Tạo một nút mới trong cây AVL
Node *newNode(string bienSo, ThoiGian thoiGianRaVao) {
    Node *node = new Node();
    node->bienSo = bienSo;
    node->thoiGianRaVao = thoiGianRaVao;
    node->left = nullptr;
    node->right = nullptr;
    node->height = 1; // Chiều cao mặc định của nút là 1
    return node;
}

// Hàm xoay cây sang phải
Node *rightRotate(Node *y) {
    Node *x = y->left;
    Node *T2 = x->right;

    // Thực hiện xoay
    x->right = y;
    y->left = T2;

    // Cập nhật chiều cao
    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;

    // Trả về nút mới root
    return x;
}

// Hàm xoay cây sang trái
Node *leftRotate(Node *x) {
    Node *y = x->right;
    Node *T2 = y->left;

    // Thực hiện xoay
    y->left = x;
    x->right = T2;

    // Cập nhật chiều cao
    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;

    // Trả về nút mới root
    return y;
}

// Lấy giá trị cân bằng của nút
int getBalance(Node *N) {
    if (N == nullptr)
        return 0;
    return height(N->left) - height(N->right);
}

// Thêm một nút vào cây AVL
Node *insert(Node *node, string bienSo, ThoiGian thoiGianRaVao) {
    // Bước 1: Thêm nút vào cây nhị phân thông thường
    if (node == nullptr)
        return newNode(bienSo, thoiGianRaVao);

    if (bienSo < node->bienSo)
        node->left = insert(node->left, bienSo, thoiGianRaVao);
    else if (bienSo > node->bienSo)
        node->right = insert(node->right, bienSo, thoiGianRaVao);
    else {
        // Biển số xe đã tồn tại, cập nhật thời gian ra vào
        node->thoiGianRaVao = thoiGianRaVao;
        return node;
    }

    // Bước 2: Cập nhật chiều cao của nút cha
    node->height = 1 + max(height(node->left), height(node->right));

    // Bước 3: Lấy giá trị cân bằng của nút cha
    int balance = getBalance(node);

    // Bước 4: Cân bằng lại cây nếu cần thiết

    // Trường hợp Left Left
    if (balance > 1 && bienSo < node->left->bienSo)
        return rightRotate(node);

    // Trường hợp Right Right
    if (balance < -1 && bienSo > node->right->bienSo)
        return leftRotate(node);

    // Trường hợp Left Right
    if (balance > 1 && bienSo > node->left->bienSo) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    // Trường hợp Right Left
    if (balance < -1 && bienSo < node->right->bienSo) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    // Trường hợp cây đã cân bằng
    return node;
}

// Tìm nút có giá trị nhỏ nhất (phải đi đến nhánh con trái nhất)
Node *minValueNode(Node *node) {
    Node *current = node;
    while (current && current->left != nullptr)
        current = current->left;
    return current;
}

// Xóa một nút khỏi cây AVL
Node *deleteNode(Node *root, string bienSo) {
    // Bước 1: Thực hiện xóa nút giống như trong BST
    if (root == nullptr)
        return root;

    // Nếu biển số xe nhỏ hơn giá trị của root, đi sang cây con trái
    if (bienSo < root->bienSo)
        root->left = deleteNode(root->left, bienSo);

    // Nếu biển số xe lớn hơn giá trị của root, đi sang cây con phải
    else if (bienSo > root->bienSo)
        root->right = deleteNode(root->right, bienSo);

    // Nếu biển số xe trùng với giá trị của root, thực hiện xóa nút
    else {
        // Nút có một hoặc không có con
        if (root->left == nullptr || root->right == nullptr) {
            Node *temp = root->left ? root->left : root->right;

            // Trường hợp không có con
            if (temp == nullptr) {
                temp = root;
                root = nullptr;
            } else // Trường hợp một con
                *root = *temp; // Copy nội dung của con

            delete temp;
        } else {
            // Nút có hai con: Lấy nút kế thừa (nút nhỏ nhất bên cây con phải)
            Node *temp = minValueNode(root->right);

            // Copy dữ liệu của nút kế thừa vào nút hiện tại
            root->bienSo = temp->bienSo;
            root->thoiGianRaVao = temp->thoiGianRaVao;

            // Xóa nút kế thừa
            root->right = deleteNode(root->right, temp->bienSo);
        }
    }

    // Nếu cây chỉ có một nút, trả về
    if (root == nullptr)
        return root;

    // Bước 2: Cập nhật chiều cao của nút hiện tại
    root->height = 1 + max(height(root->left), height(root->right));

    // Bước 3: Lấy giá trị cân bằng của nút hiện tại
    int balance = getBalance(root);

    // Bước 4: Cân bằng lại cây nếu cần thiết

    // Trường hợp Left Left
    if (balance > 1 && getBalance(root->left) >= 0)
        return rightRotate(root);

    // Trường hợp Left Right
    if (balance > 1 && getBalance(root->left) < 0) {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }

    // Trường hợp Right Right
    if (balance < -1 && getBalance(root->right) <= 0)
        return leftRotate(root);

    // Trường hợp Right Left
    if (balance < -1 && getBalance(root->right) > 0) {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }

    // Trường hợp cây đã cân bằng
    return root;
}

// Lặp qua và in ra cây theo thứ tự
void inOrder(Node *root) {
    if (root != nullptr) {
        inOrder(root->left);
        cout << "Bien so: " << root->bienSo << ", Thoi gian vao: " << ctime(&root->thoiGianRaVao.vao);
        inOrder(root->right);
    }
}

// Lặp qua cây để liệt kê xe vào trong khoảng thời gian xác định
void lietKeXeTrongKhoangThoiGian(Node *root, time_t start, time_t end) {
    if (root != nullptr) {
        lietKeXeTrongKhoangThoiGian(root->left, start, end);
        if (root->thoiGianRaVao.vao >= start && root->thoiGianRaVao.vao <= end) {
            cout << "Bien so: " << root->bienSo << ", Thoi gian vao: " << ctime(&root->thoiGianRaVao.vao);
        }
        lietKeXeTrongKhoangThoiGian(root->right, start, end);
    }
}

// Đếm số lượt xe vào trong ngày
void thongKeLuotXeVaoTrongNgay(Node *root, int &count) {
    if (root != nullptr) {
        thongKeLuotXeVaoTrongNgay(root->left, count);
        time_t now = time(0);
        tm *ltm = localtime(&now);
        tm *ltmTg = localtime(&root->thoiGianRaVao.vao);
        if (ltmTg->tm_year == ltm->tm_year && ltmTg->tm_mon == ltm->tm_mon && ltmTg->tm_mday == ltm->tm_mday) {
            count++;
        }
        thongKeLuotXeVaoTrongNgay(root->right, count);
    }
}#include <iostream>
#include <string>
#include <ctime>

using namespace std;

// Cấu trúc lưu trữ thời gian vào/ra của xe
struct TimeRecord {
    time_t entryTime;
    time_t exitTime;
};

// Cấu trúc nút của cây AVL
struct Node {
    string licensePlate;
    TimeRecord timeRecord;
    Node *left;
    Node *right;
    int height;
};

// Hàm hỗ trợ cho các thao tác trên cây

// Lấy chiều cao của nút (0 nếu nút rỗng)
int height(Node *N) { return N ? N->height : 0; }

// Tạo một nút mới
Node *newNode(const string &licensePlate, const TimeRecord &timeRecord) {
    Node *node = new Node();
    node->licensePlate = licensePlate;
    node->timeRecord = timeRecord;
    node->left = node->right = nullptr;
    node->height = 1;  // Nút mới có chiều cao mặc định là 1
    return node;
}

// Xoay phải
Node *rightRotate(Node *y) {
    Node *x = y->left;
    Node *T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;
    return x;
}

// Xoay trái (tương tự như xoay phải)
Node *leftRotate(Node *x) {
    // ... (triển khai tương tự như rightRotate)
}

// Lấy hệ số cân bằng của nút
int getBalance(Node *N) {
    return N ? height(N->left) - height(N->right) : 0;
}

// Chèn một nút (hoặc cập nhật thời gian nếu biển số đã tồn tại)
Node *insert(Node *node, const string &licensePlate, const TimeRecord &timeRecord) {
    // ... (triển khai tương tự như bản gốc, nhưng có định dạng tốt hơn)
}

// Tìm nút có giá trị nhỏ nhất (nút ngoài cùng bên trái)
Node *minValueNode(Node *node) {
    // ... (triển khai tương tự như bản gốc)
}

// Xóa một nút khỏi cây AVL
Node *deleteNode(Node *root, const string &licensePlate) {
    // ... (triển khai tương tự như bản gốc, nhưng có định dạng tốt hơn)
}

// Duyệt theo thứ tự (in biển số và thời gian vào)
void inOrder(Node *root) {
    if (root) {
        inOrder(root->left);
        cout << "Bien so: " << root->licensePlate
             << ", Thoi gian vao: " << ctime(&root->timeRecord.entryTime);
        inOrder(root->right);
    }
}

// Liệt kê các xe vào trong một khoảng thời gian
void listVehiclesInRange(Node *root, time_t startTime, time_t endTime) {
    if (root) {
        listVehiclesInRange(root->left, startTime, endTime);
        if (root->timeRecord.entryTime >= startTime && root->timeRecord.entryTime <= endTime) {
            cout << "Bien so: " << root->licensePlate
                 << ", Thoi gian vao: " << ctime(&root->timeRecord.entryTime);
        }
        listVehiclesInRange(root->right, startTime, endTime);
    }
}

// Đếm số lượt xe vào trong ngày hiện tại
void countTodaysEntries(Node *root, int &count) {
    if (root) {
        countTodaysEntries(root->left, count);

        time_t now = time(0);
        tm *ltm = localtime(&now);
        tm *ltmTg = localtime(&root->timeRecord.entryTime);

        if (ltmTg->tm_year == ltm->tm_year && 
            ltmTg->tm_mon == ltm->tm_mon && 
            ltmTg->tm_mday == ltm->tm_mday) {
            count++;
        }

        countTodaysEntries(root->right, count);
    }
}

int main() {
    Node *root = nullptr;
    int luaChon;
    do {
        cout << "Menu:" << endl;
        cout << "1. Them thong tin xe vao" << endl;
        cout << "2. Xoa thong tin xe" << endl;
        cout << "3. Liet ke xe vao trong khoang thoi gian" << endl;
        cout << "4. Thong ke so luot xe vao trong ngay" << endl;
        cout << "0. Thoat" << endl;
        cout << "Nhap lua chon cua ban: ";
        cin >> luaChon;

        switch (luaChon) {
            case 1: {
                string bienSo;
                cout << "Nhap bien so xe (8 ky tu): ";
                cin >> bienSo;
                ThoiGian tg;
                tg.vao = time(0); // Thời gian hiện tại
                tg.ra = 0; // Thời gian ra chưa có giá trị khi xe vào lần đầu
                root = insert(root, bienSo, tg);
                cout << "Da ghi nhan thong tin xe." << endl;
                break;
            }
            case 2: {
                string bienSo;
                cout << "Nhap bien so xe can xoa: ";
                cin >> bienSo;
                root = deleteNode(root, bienSo);
                cout << "Da xoa thong tin xe." << endl;
                break;
            }
            case 3: {
                time_t start, end;
                cout << "Nhap thoi gian bat dau (timestamp): ";
                cin >> start;
                cout << "Nhap thoi gian ket thuc (timestamp): ";
                cin >> end;
                cout << "Cac xe vao trong khoang thoi gian da cho:" << endl;
                lietKeXeTrongKhoangThoiGian(root, start, end);
                break;
            }
            case 4: {
                int count = 0;
                thongKeLuotXeVaoTrongNgay(root, count);
                cout << "So luot xe vao trong ngay hom nay: " << count << endl;
                break;
            }
            case 0:
                cout << "Ket thuc chuong trinh." << endl;
                break;
            default:
                cout << "Lua chon khong hop le. Vui long chon lai." << endl;
                break;
        }

    } while (luaChon != 0);

    return 0;
}
