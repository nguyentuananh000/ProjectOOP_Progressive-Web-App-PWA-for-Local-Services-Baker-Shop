#include<iostream>
#include<string>
#include<vector>

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
    //Tạo một acc mới
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
//-------------------Customer kế thừa từ User------------------------//

class Customer : public User {
private:
    string address;
    int loyaltyPoints;
    string defaultPaymentMethod;
    map<string, int> cart;
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
    void addToCart(string product, int quantity) {
        cart[product] += quantity;
        cout << "Added " << quantity << " x " << product << " to cart.\n";
    }
    //cập nhật số lượng vật phẩm
    void updateCart(string product, int newQuantity) {
        if (cart.count(product)) {
            cart[product] = newQuantity;
            cout << "Updated " << product << " quantity to " << newQuantity << endl;
        } else {
            cout << "Product not found in cart.\n";
        }
    }
    //xoá vật phẩm trong giỏ hàng
    void removeFromCart(string product) {
        if (cart.erase(product))
            cout << "Removed " << product << " from cart.\n";
        else
            cout << "Product not found in cart.\n";
    }
    //xem giỏ hàng
    void viewCart() {
        if (cart.empty()) {
            cout << "Your cart is empty.\n";
            return;
        }
        cout << "Your cart:\n";
        for (auto item : cart) {
            cout << "- " << item.first << ": " << item.second << "\n";
        }
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
};
int main() {
    
    
    
}
