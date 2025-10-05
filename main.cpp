#include <iostream>
#include <ctime>
#include <vector>
#include <map>
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
int main()
{
    
}
