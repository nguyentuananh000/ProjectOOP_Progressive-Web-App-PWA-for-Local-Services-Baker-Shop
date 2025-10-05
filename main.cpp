#include <iostream>
#include <ctime>
#include <vector>
#include <map>
#include <string>
using namespace std;
class User {
protected:
    string userId;
    string name;
    string email;
    string phone;
    string passwordHash;
    string status;
    string role;
    time_t createdAt;
    time_t lastLoginAt;
    bool twoFactorEnabled;
public:
    //Tạo một acc mới (hàm khởi tạo)
    void registerAccount(string _name, string _email, string _phone, string _passwordHash, string _role) {
        if (_email.empty() || _phone.empty() || _passwordHash.empty() || _name.empty() || _role.empty()) {
            cout << "Registration failed: missing information.\n";
            return;
        }
        userId = "U" + to_string(rand() % 10000);
        name = _name;
        email = _email;
        phone = _phone;
        passwordHash = _passwordHash;
        status = "active";
        role = _role;
        createdAt = time(nullptr);
        lastLoginAt = time(nullptr);
        twoFactorEnabled = false;
        cout << "Registered successfully! UserID: " << userId << endl;
    }
    // Đăng nhập
    void login(string identifier, string password) {
        if ((identifier == email || identifier == phone) && password == passwordHash) {
            lastLoginAt = time(nullptr);
            cout << "Login successful! Welcome back, " << name << endl;
        } else {
            cout << "Invalid login credentials.\n";
        }
    }
    //Reset mật khẩu
    void resetPassword(string otp) {
        // Giả sử otp gửi về điện thoại hoặc email là 123456
        if(otp == "123456") {
            cout<<"Enter new password: ";
            string newPassword;
            cin>>newPassword;
            passwordHash = newPassword;
            cout<<"Password reset successful!\n";
        }
        else {
            cout<<"Invalid OTP.\n";
        }
    }
    //Chỉnh sửa hồ sơ
    void updateProfile(string _name, string _email, string _phone, string _status) {
        if (!_name.empty()) name = _name;
        if (!_email.empty()) email = _email;
        if (!_phone.empty()) phone = _phone;
        if (!_status.empty()) status = _status;
        cout << "Profile updated successfully.\n";
    }
    //Bật xác thực 2 yếu tố
    void enable2FA() {
        twoFactorEnabled = true;
        cout << "Two-factor authentication enabled.\n";
    }
    //Tắt xác thực 2 yếu tố
    void disable2FA() {
        twoFactorEnabled = false;
        cout << "Two-factor authentication disabled.\n";
    }
};
class Product {
private:
    string productId;
    string sku;
    string name;
    string description;
    double price;
    string imageUrl;
    bool isActive;
    int prepTime;
    string categoryId;
    time_t createdAt;
public:
    Product(string _productId, string _sku, string _name, string _description,
            double _price, string _imageUrl, bool _isActive,
            int _prepTime, string _categoryId)
    : productId(_productId), sku(_sku), name(_name), description(_description),
    price(_price), imageUrl(_imageUrl), isActive(_isActive),
    prepTime(_prepTime), categoryId(_categoryId)
    {
        createdAt = time(nullptr);
    }
    void setPrice(double newPrice) {
        if (newPrice <= 0) {
            cout << "Invalid price. Must be greater than zero.\n";
            return;
        }
        price = newPrice;
        cout << "Price updated to " << price << " VND.\n";
    }
    
    
    void assignCategory(string newCategoryId) {
        if (newCategoryId.empty()) {
            cout << "Invalid category ID.\n";
            return;
        }
        categoryId = newCategoryId;
        cout << "Product assigned to category: " << categoryId << endl;
    }
    
    
    void activate() {
        isActive = true;
        cout << "Product activated for sale.\n";
    }
    
    void deactivate() {
        isActive = false;
        cout << "Product deactivated from sale.\n";
    }
    string getProductId() {
        return productId;
    }
    string getName() {
        return name;
    }
    double getPrice() {
        return price;
    }
    bool getIsActive() {
        return isActive;
    }
    string getCategoryId() {
        return categoryId;
    }
    void displayInfo() {
        cout << "Product ID: " << productId << endl;
        cout << "SKU: " << sku << endl;
        cout << "Name: " << name << endl;
        cout << "Description: " << description << endl;
        cout << "Price: " << price << " VND" << endl;
        cout << "Preparation Time: " << prepTime << " mins" << endl;
        cout << "Category: " << categoryId << endl;
        cout << "Image: " << imageUrl << endl;
        cout << "Status: " << (isActive ? "Available" : "Discontinued") << endl;
    }
    
};
//-------------------Customer kế thừa từ User------------------------//

class Customer : public User {
private:
    string address;
    int loyaltyPoints;
    string defaultPaymentMethod;
    vector<pair<Product, int>> cart;
    string currentOrderStatus;
public:
    // Hàm khởi tạo
    Customer() {
        loyaltyPoints = 0;
        defaultPaymentMethod = "Cash on Delivery";
    }
    //cập nhật địa chỉ
    void updateAddress(string _address) {
        address = _address;
        cout << "Address added successfully!\n";
    }
    //
    void setDefaultPaymentMethod() {
        
    }
    //tính điểm tích luỹ
    void earnPoints(int points) {
        loyaltyPoints += points;
        cout << "Earned " << points << " points. Total: " << loyaltyPoints << endl;
    }
    //quy đổi điểm tích luỹ
    void redeemPoints(int points) {
        if (points <= loyaltyPoints) {
            loyaltyPoints -= points;
            cout << "Redeemed " << points << " points. Remaining: " << loyaltyPoints << endl;
        } else {
            cout << "Not enough points to redeem.\n";
        }
    }
    //thêm vào giỏ hàng
    void addToCart(Product product, int quantity) {
        if (quantity <= 0) {
            cout << "Quantity must be greater than 0.\n";
            return;
        }
        
        // Nếu sản phẩm đã tồn tại trong giỏ, tăng số lượng
        for (auto item : cart) {
            if (item.first.getProductId() == product.getProductId()) {
                item.second += quantity;
                cout << "Updated quantity for " << product.getName()
                << " to " << item.second << endl;
                return;
            }
        }
        
        // Nếu chưa có, thêm sản phẩm mới
        cart.push_back({product, quantity});
        cout << "Added " << quantity << " x " << product.getName() << " to cart.\n";
    }
    //cập nhật số lượng vật phẩm
    void updateCart(string productId, int newQuantity) {
        for (auto item : cart) {
            if (item.first.getProductId() == productId) {
                if (newQuantity <= 0) {
                    cout << "Invalid quantity. Use removeFromCart() to delete item.\n";
                    return;
                }
                item.second = newQuantity;
                cout << "Updated " << item.first.getName()
                << " quantity to " << newQuantity << endl;
                return;
            }
        }
        cout << "Product not found in cart.\n";
    }
    //xoá vật phẩm trong giỏ hàng
    void removeFromCart(string productId) {
        for (auto it = cart.begin(); it != cart.end(); ++it) {
            if (it->first.getProductId() == productId) {
                cout << "Removed " << it->first.getName() << " from cart.\n";
                cart.erase(it);
                return;
            }
        }
        cout << "Product not found in cart.\n";
    }
    //xem giỏ hàng
    void viewCart() {
        if (cart.empty()) {
            cout << "Your cart is empty.\n";
            return;
        }
        cout << "\n--- Your Cart ---\n";
        double total = 0;
        for (auto item : cart) {
            double subTotal = item.first.getPrice() * item.second;
            total += subTotal;
            cout << item.first.getName() << " x " << item.second
            << " → " << subTotal << " VND\n";
        }
        cout << "------------------\n";
        cout << "Total: " << total << " VND\n";
    }
    //đặt hàng
    void placeOrder(bool homeDelivery) {
        if (cart.empty()) {
            cout << "Cart is empty. Cannot place order.\n";
            return;
        }
        earnPoints(10);
        cart.clear();
        currentOrderStatus = "Pending Confirmation";
        cout << "Order placed successfully! Delivery: "
        << (homeDelivery ? "Home Delivery" : "In-store Pickup") << endl;
    }
    //xem trạng thái đơn hàng
    void trackOrder() {
        cout << "Current order status: " << currentOrderStatus << endl;
    }
    //cập nhật trạng thái đơn hàng
    void updateOrderStatus(string status) {
        currentOrderStatus = status;
        cout << "Order status updated to: " << status << endl;
    }
    //Xem các mặt hàng
    void browseProducts(vector<Product> products) {
        if (products.empty()) {
            cout << "No products available.\n";
            return;
        }
        bool foundActive = false;
        cout << "\n--- Product Catalog ---\n";
        for (auto p : products) {
            if (p.getIsActive()) {
                p.displayInfo();
                foundActive = true;
            }
        }
        if (!foundActive)
            cout << "No active products available.\n";
    }
    //Tìm mặt hàng
    void searchProduct(vector<Product> products, string name) {
        for(auto p: products) {
            if(p.getName() == name) {
                p.displayInfo();
                return;
            }
        }
        cout<<"No products found matching your search.\n";
    }
};
//--------------------------------Guest---------------------//
class GuestUser {
private:
    string sessionId; // Mã định danh tạm cho khách truy cập
public:
    // --- Constructor ---
    GuestUser() {
        sessionId = "G" + to_string(rand() % 10000);
        cout << "Guest session started: " << sessionId << endl;
    }
    
    //Xem các mặt hàng
    void browseProducts(vector<Product> products) {
        if (products.empty()) {
            cout << "No products available.\n";
            return;
        }
        cout << "\n--- Product Catalog ---\n";
        for (auto p : products) {
            if (p.getIsActive()) {
                p.displayInfo();
            }
        }
        cout<<'\n';
    }
    
    //đăng kí trở thành Customer
    Customer registerAsCustomer(string name, string email, string phone, string password) {
        Customer newCustomer;
        newCustomer.registerAccount(name, email, phone, password, "customer");
        cout << "Welcome, " << name << "! Your customer account has been created.\n";
        return newCustomer;
    }
};
//------------------- Admin kế thừa từ User ------------------------//
class Admin : public User {
private:
    vector<Product*>* product { NULL }; // danh sách sản phẩm (dùng chung)
    vector<User*>*    user    { NULL }; // (để dành nếu muốn quản lý tài khoản)
    vector<string>    promotion;        // danh sách mã khuyến mãi

public:
    Admin(vector<Product*>* p = NULL, vector<User*>* u = NULL) {
        product = p;
        user = u;
        role = "admin"; // thể hiện kế thừa từ User
    }

    void setProductList(vector<Product*>* p) { product = p; }
    void setUserList(vector<User*>* u) { user = u; }

    // 1) Tạo sản phẩm mới
    void createProduct(string _productId, string _sku, string _name, string _description,
                       double _price, string _imageUrl, bool _isActive,
                       int _prepTime, string _categoryId) {
        if (!product) {
            cout << "[ADMIN] Product list not initialized.\n";
            return;
        }
        // tránh trùng ID
        for (int i = 0; i < (int)product->size(); i++) {
            if ((*product)[i]->getProductId() == _productId) {
                cout << "[ADMIN] Duplicate productId.\n";
                return;
            }
        }
        Product* pNew = new Product(_productId,_sku,_name,_description,_price,_imageUrl,_isActive,_prepTime,_categoryId);
        product->push_back(pNew);
        cout << "[ADMIN] Product created: " << _name << "\n";
    }

    // 2) Cập nhật sản phẩm (giá, category, kích hoạt/ngưng bán)
    void updateProduct(string productId, double newPrice, string newCategoryId, bool activate) {
        if (!product) {
            cout << "[ADMIN] Product list not initialized.\n";
            return;
        }
        for (int i = 0; i < (int)product->size(); i++) {
            Product* p = (*product)[i];
            if (p->getProductId() == productId) {
                if (newPrice > 0) p->setPrice(newPrice);
                if (newCategoryId != "") p->assignCategory(newCategoryId);
                if (activate) p->activate(); else p->deactivate();
                cout << "[ADMIN] Product " << productId << " updated.\n";
                return;
            }
        }
        cout << "[ADMIN] Product not found.\n";
    }

    // 3) XÓA MỀM sản phẩm (giữ lịch sử)
    void deleteProduct(string productId) {
        if (!product) {
            cout << "[ADMIN] Product list not initialized.\n";
            return;
        }
        for (int i = 0; i < (int)product->size(); i++) {
            Product* p = (*product)[i];
            if (p->getProductId() == productId) {
                p->deactivate();
                cout << "[ADMIN] Product " << productId << " deactivated (soft-delete).\n";
                return;
            }
        }
        cout << "[ADMIN] Product not found.\n";
    }

    // (tuỳ chọn) XÓA CỨNG sản phẩm — chỉ dùng khi thực sự muốn gỡ khỏi bộ nhớ
    void hardDeleteProduct(string productId) {
        if (!product) { cout << "[ADMIN] Product list not initialized.\n"; return; }
        for (int i = 0; i < (int)product->size(); i++) {
            if ((*product)[i]->getProductId() == productId) {
                delete (*product)[i];
                product->erase(product->begin() + i);
                cout << "[ADMIN] Product " << productId << " permanently deleted.\n";
                return;
            }
        }
        cout << "[ADMIN] Product not found.\n";
    }

    // 4) Thêm mã khuyến mãi
    void managePromotions(string promo) {
        promotion.push_back(promo);
        cout << "[ADMIN] Promotion added: " << promo << "\n";
    }

    // 5) Áp dụng khuyến mãi
    void applyPromotion(string promo) {
        for (int i = 0; i < (int)promotion.size(); i++) {
            if (promotion[i] == promo) {
                cout << "[ADMIN] Promotion applied: " << promo << "\n";
                return;
            }
        }
        cout << "[ADMIN] Promotion not found.\n";
    }

    // 6) Hết hạn/loại bỏ khuyến mãi
    void expirePromotion(string promo) {
        for (int i = 0; i < (int)promotion.size(); i++) {
            if (promotion[i] == promo) {
                promotion.erase(promotion.begin() + i);
                cout << "[ADMIN] Promotion expired: " << promo << "\n";
                return;
            }
        }
        cout << "[ADMIN] Promotion not found.\n";
    }

    // 7) (tuỳ chọn) Xem toàn bộ sản phẩm
    void viewAllProducts() {
        if (!product) return;
        cout << "\n--- All Products ---\n";
        for (int i = 0; i < (int)product->size(); i++) {
            (*product)[i]->displayInfo();
        }
        cout << '\n';
    }
};
// ======================= ORDER (chuẩn hoá trạng thái) =======================
struct Order {
    string orderId;
    string customerId;
    vector<pair<Product, int> > items; // Sản phẩm và số lượng
    double totalAmount;
    string status; // "pending", "confirmed", "in_progress", "finished", "completed"
    time_t createdAt;
    time_t updatedAt;

    Order() {
        totalAmount = 0;
        status = "pending";
        createdAt = time(NULL);
        updatedAt = createdAt;
    }
};

// ======================= STAFF (kế thừa User) =======================
class Staff : public User {
private:
    vector<Order>* orders;   // con trỏ đến danh sách đơn hàng
    string currentOrderId;   // đơn đang xử lý
    string workStatus;       // "idle", "working"

public:
    Staff(vector<Order>* o = NULL) {
        orders = o;
        role = "staff";
        currentOrderId = "";
        workStatus = "idle";
    }

    void setOrderList(vector<Order>* o) { orders = o; }

    // Nhân viên nhận đơn: chỉ nhận khi đơn "confirmed"
    void claimOrder(string orderId) {
        if (!orders) {
            cout << "[STAFF] Order list not initialized.\n";
            return;
        }
        for (int i = 0; i < (int)orders->size(); i++) {
            if ((*orders)[i].orderId == orderId) {
                if ((*orders)[i].status != "confirmed") {
                    cout << "[STAFF] Order must be 'confirmed' to claim.\n";
                    return;
                }
                currentOrderId = orderId;
                workStatus = "working";
                cout << "[STAFF] Order " << orderId << " claimed successfully.\n";
                return;
            }
        }
        cout << "[STAFF] Order not found.\n";
    }

    // Cập nhật tiến độ: confirmed -> in_progress -> finished
    void updateOrderStatus(string newStatus) {
        if (!orders) {
            cout << "[STAFF] Order list not initialized.\n";
            return;
        }
        if (currentOrderId == "") {
            cout << "[STAFF] No current order claimed.\n";
            return;
        }
        for (int i = 0; i < (int)orders->size(); i++) {
            if ((*orders)[i].orderId == currentOrderId) {
                string old = (*orders)[i].status;

                // Chặn completed ở đây (để dành cho markOrderCompleted)
                if (newStatus == "completed") {
                    cout << "[STAFF] Use markOrderCompleted() after 'finished'.\n";
                    return;
                }

                // Chuyển trạng thái hợp lệ
                if (old == "confirmed" && newStatus == "in_progress") {
                    (*orders)[i].status = "in_progress";
                    (*orders)[i].updatedAt = time(NULL);
                    cout << "[STAFF] Order " << currentOrderId << " -> in_progress\n";
                    return;
                }
                if (old == "in_progress" && newStatus == "finished") {
                    (*orders)[i].status = "finished";
                    (*orders)[i].updatedAt = time(NULL);
                    cout << "[STAFF] Order " << currentOrderId << " -> finished\n";
                    return;
                }

                cout << "[STAFF] Invalid status transition: " << old << " -> " << newStatus << "\n";
                return;
            }
        }
        cout << "[STAFF] Current order not found.\n";
    }

    // Hoàn tất đơn: chỉ khi đã "finished"
    void markOrderCompleted() {
        if (!orders) {
            cout << "[STAFF] Order list not initialized.\n";
            return;
        }
        if (currentOrderId == "") {
            cout << "[STAFF] No current order claimed.\n";
            return;
        }
        for (int i = 0; i < (int)orders->size(); i++) {
            if ((*orders)[i].orderId == currentOrderId) {
                if ((*orders)[i].status == "finished") {
                    (*orders)[i].status = "completed";
                    (*orders)[i].updatedAt = time(NULL);
                    cout << "[STAFF] Order " << currentOrderId << " marked as completed.\n";
                    // reset trạng thái staff
                    currentOrderId = "";
                    workStatus = "idle";
                    return;
                } else {
                    cout << "[STAFF] Order not in 'finished' state.\n";
                    return;
                }
            }
        }
        cout << "[STAFF] Current order not found.\n";
    }

    // Xem trạng thái làm việc
    void viewWorkStatus() {
        cout << "[STAFF] Work: " << workStatus;
        if (currentOrderId != "") cout << " | Current order: " << currentOrderId;
        cout << "\n";
    }
};

int main() {
    srand(time(0));

    // Danh sách sản phẩm (để Admin quản lý)
    vector<Product*> productList;

    // Danh sách user (tuỳ bạn dùng)
    vector<User*> userList;

    // Admin (dùng lại class Admin bạn đã có ở trên)
    Admin admin(&productList, &userList);

    // Tạo vài sản phẩm
    admin.createProduct("P01","SKU01","Chocolate Cake","",50000,"",true,30,"C01");
    admin.createProduct("P02","SKU02","Cheese Cake","",60000,"",true,30,"C02");
    admin.updateProduct("P02", 65000, "C02", true);
    admin.deleteProduct("P01"); // xóa mềm
    admin.managePromotions("NEW10");
    admin.applyPromotion("NEW10");

    // Danh sách đơn
    vector<Order> orderList;
    Order o1; 
    o1.orderId = "ORD01";
    o1.customerId = "CUST01";
    o1.status = "confirmed";   // giả lập: Admin đã xác nhận xong
    orderList.push_back(o1);

    // Staff
    Staff baker(&orderList);

    // Test 1: Claim order khi "confirmed"
    baker.claimOrder("ORD01");          // ok
    baker.viewWorkStatus();

    // Test 2: Cập nhật "confirmed" -> "in_progress"
    baker.updateOrderStatus("in_progress"); // ok

    // Test 3: Cập nhật "in_progress" -> "finished"
    baker.updateOrderStatus("finished");    // ok

    // Test 4: Đánh dấu completed (chỉ khi "finished")
    baker.markOrderCompleted();             // ok

    // Test 5: Claim lại đơn đã completed (sẽ fail)
    baker.claimOrder("ORD01");              // fail vì không còn "confirmed"

    // In trạng thái đơn cuối
    cout << "[CHECK] ORD01 final status: " << orderList[0].status << "\n";

    // Dọn bộ nhớ sản phẩm cấp phát
    for (int i = 0; i < (int)productList.size(); i++) delete productList[i];
    return 0;
}
