#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <cstdlib>
#include <utility>

using namespace std;

struct OrderItem {
    string productId;
    string productName;
    int quantity;
    double priceAtPurchase;

    OrderItem(string id, string name, int q, double price)
        : productId(id), productName(name), quantity(q), priceAtPurchase(price) {}
};

struct Order {
    string orderId;
    string customerId;
    vector<OrderItem> items;
    double totalAmount;
    string status;
    time_t createdAt;
    time_t updatedAt;

    Order() {
        totalAmount = 0;
        status = "pending";
        createdAt = time(nullptr);
        updatedAt = createdAt;
    }
};

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
    virtual ~User() {}
    User() {
        createdAt = time(nullptr);
        lastLoginAt = createdAt;
        twoFactorEnabled = false;
        status = "inactive";
    }
    
    virtual bool registerAccount(string _name, string _email, string _phone, string _password, string _role) = 0;
    virtual void login(string identifier, string password) = 0;
    
    void resetPassword(string otp) {
        if (otp == "123456") {
            string newPassword;
            cout << "Enter new password: ";
            cin >> newPassword;
            passwordHash = newPassword;
            cout << "Password reset successful.\n";
        } else cout << "Invalid OTP.\n";
    }
    
    void updateProfile(string _name, string _email, string _phone, string _status) {
        if (!_name.empty()) name = _name;
        if (!_email.empty()) email = _email;
        if (!_phone.empty()) phone = _phone;
        if (!_status.empty()) status = _status;
        cout << "Profile updated successfully.\n";
    }
    
    void enable2FA() {
        twoFactorEnabled = true;
        cout << "Two-factor authentication enabled.\n";
    }
    
    void disable2FA() {
        twoFactorEnabled = false;
        cout << "Two-factor authentication disabled.\n";
    }
    
    virtual void displayInfo() const {
        cout << "User ID: " << userId << endl;
        cout << "Name: " << name << endl;
        cout << "Email: " << email << endl;
        cout << "Phone: " << phone << endl;
        cout << "Role: " << role << endl;
        cout << "Status: " << status << endl;
        cout << "Created At: " << (createdAt ? ctime(&createdAt) : string("N/A\n"));
        cout << "Last Login At: " << (lastLoginAt ? ctime(&lastLoginAt) : string("N/A\n"));
        cout << "2FA: " << (twoFactorEnabled ? "Enabled" : "Disabled") << endl;
    }
    
    string getName() const { return name; }
    string getRole() const { return role; }
    string getUserId() const { return userId; }
};

class Product {
private:
    string productId;
    string name;
    string description;
    double price;
    bool isActive;
    int quantityStock;
public:
    Product(string _productId, string _name, double _price, bool _isActive, string _description = "", int _quantityStock = 0)
    : productId(_productId), name(_name), price(_price), isActive(_isActive), description(_description), quantityStock(_quantityStock) {}
    
    string getProductId() const { return productId; }
    string getName() const { return name; }
    double getPrice() const { return price; }
    bool getIsActive() const { return isActive; }
    int getQuantityStock() const { return quantityStock; }
    
    void displayInfo() const {
        cout << "Product ID: " << productId << endl;
        cout << "Name: " << name << endl;
        cout << "Price: " << price << " VND" << endl;
        cout << "Description: " << description << endl;
        cout << "Stock: " << quantityStock << " | Status: " << (isActive ? "Available" : "Unavailable") << endl;
    }

    void setPrice(double newPrice) {
        if (newPrice >= 0) price = newPrice;
    }
    void activate() {
        isActive = true;
    }
    void deactivate() {
        isActive = false;
    }
    void setQuantityStock(int newStock) {
        if (newStock >= 0) quantityStock = newStock;
    }
    void setDescription(string newDesc) {
        description = newDesc;
    }
};

class Customer : public User {
private:
    string address;
    int loyaltyPoints;
    vector<pair<string, int>> cart;

public:
    Customer() {
        address = "";
        loyaltyPoints = 0;
        status = "active";
        role = "customer";
    }
    
    bool registerAccount(string _name, string _email, string _phone, string _password, string _role) override {
        if (_name.empty() || _email.empty() || _phone.empty() || _password.empty() || _role.empty()) {
            cout << "[Customer] Registration failed: missing information.\n";
            return false;
        }
        userId = "C" + to_string(rand() % 10000 + 1000);
        name = _name;
        email = _email;
        phone = _phone;
        passwordHash = _password;
        role = _role;
        status = "active";
        createdAt = time(nullptr);
        lastLoginAt = createdAt;
        cout << "[Customer] Registered successfully! UserID: " << userId << endl;
        return true;
    }
    
    void login(string identifier, string password) override {
        if ((identifier == email || identifier == phone) && password == passwordHash) {
            lastLoginAt = time(nullptr);
            cout << "[Customer] Login successful! Welcome back, " << name << endl;
        } else cout << "[Customer] Invalid login credentials.\n";
    }
    
    void updateAddress(string _address) {
        address = _address;
        cout << "Address updated: " << address << endl;
    }
    
    void earnPoints(int p) {
        loyaltyPoints += p;
        cout << "Earned " << p << " points. Total: " << loyaltyPoints << endl;
    }
    
    void redeemPoints(int p) {
        if (p <= loyaltyPoints) {
            loyaltyPoints -= p;
            cout << "Redeemed " << p << " points. Remaining: " << loyaltyPoints << endl;
        } else cout << "Not enough points.\n";
    }
    
    void addToCart(const vector<Product>& availableProducts, string productName, int quantity) {
        if (quantity <= 0) {
            cout << "Invalid quantity.\n";
            return;
        }
        
        const Product* productToAdd = nullptr;
        for (const auto& p : availableProducts) {
            if (p.getName() == productName) {
                productToAdd = &p;
                break;
            }
        }
        
        if (productToAdd == nullptr) {
            cout << "Product '" << productName << "' not found in the catalog. Cannot add to cart.\n";
            return;
        }
        
        if (!productToAdd->getIsActive() || productToAdd->getQuantityStock() == 0) {
            cout << "Product '" << productName << "' is currently unavailable (Out of Stock).\n";
            return;
        }
        
        int currentCartQuantity = 0;
        for (const auto &item : cart) {
            if (item.first == productToAdd->getProductId()) {
                currentCartQuantity = item.second;
                break;
            }
        }
        
        if (currentCartQuantity + quantity > productToAdd->getQuantityStock()) {
            cout << "Cannot add " << quantity << " of " << productName
            << ". Only " << productToAdd->getQuantityStock() - currentCartQuantity << " remaining in stock.\n";
            return;
        }
        
        for (auto &item : cart) {
            if (item.first == productToAdd->getProductId()) {
                item.second += quantity;
                cout << "Updated quantity for " << productToAdd->getName() << " to " << item.second << endl;
                return;
            }
        }
        
        cart.push_back({productToAdd->getProductId(), quantity});
        cout << "Added " << quantity << " x " << productToAdd->getName() << " to cart. \n";
    }
    
    void removeFromCart(const vector<Product>& availableProducts, string productName) {
        string productIdToRemove = "";
        for(const auto& p : availableProducts) {
            if(p.getName() == productName) {
                productIdToRemove = p.getProductId();
                break;
            }
        }

        for (auto it = cart.begin(); it != cart.end(); ++it) {
            if (it->first == productIdToRemove) {
                cout << "Removed " << productName << " from cart.\n";
                cart.erase(it);
                return;
            }
        }
        cout << "Product '" << productName << "' not found in cart.\n";
    }
    
    void viewCart(const vector<Product>& availableProducts) const {
        if (cart.empty()) { cout << "Your cart is empty.\n"; return; }
        double total = 0;
        cout << "\n--- Your Cart ---\n";
        
        for (const auto& item : cart) {
            const Product* p = nullptr;
            for(const auto& prod : availableProducts) {
                if(prod.getProductId() == item.first) {
                    p = &prod;
                    break;
                }
            }

            if(p) {
                double sub = p->getPrice() * item.second;
                total += sub;
                cout << p->getName() << " x " << item.second << " -> " << sub << " VND\n";
            }
        }
        cout << "Total: " << total << " VND\n";
    }
    
    void placeOrder(vector<Order>& allOrders, const vector<Product>& availableProducts, bool delivery) {
        if (cart.empty()) { cout << "Cart empty.\n"; return; }
        
        Order newOrder;
        newOrder.orderId = "O" + to_string(rand() % 10000 + 1000);
        newOrder.customerId = this->userId;

        for (const auto& cartItem : cart) {
            const Product* p = nullptr;
            for(const auto& prod : availableProducts) {
                if(prod.getProductId() == cartItem.first) {
                    p = &prod;
                    break;
                }
            }

            if(p) {
                double price = p->getPrice();
                int qty = cartItem.second;
                newOrder.items.push_back(OrderItem(p->getProductId(), p->getName(), qty, price));
                newOrder.totalAmount += price * qty;
            }
        }

        allOrders.push_back(newOrder);
        
        earnPoints(10);
        cart.clear();
        
        cout << "Order placed! ID: " << newOrder.orderId << ". Total: " << newOrder.totalAmount << " VND.\n";
    }
    
    void trackOrder(const vector<Order>& allOrders) const {
        for (auto it = allOrders.rbegin(); it != allOrders.rend(); ++it) {
            if (it->customerId == this->userId) {
                cout << "Tracking Order " << it->orderId << ": Status is " << it->status << endl;
                return;
            }
        }
        cout << "No order found for this user.\n";
    }
    
    void browseProducts(const vector<Product>& products) const {
        cout << "\n--- Product Catalog ---\n";
        for (const auto& p : products)
            if (p.getIsActive()) p.displayInfo();
    }
    
    void searchProduct(const vector<Product>& products, string name) const {
        for (const auto& p : products)
            if (p.getName() == name) { p.displayInfo(); return; }
        cout << "No product found.\n";
    }
    
    void displayInfo() const override {
        cout<<"Infomation of customer: \n";
        User::displayInfo();
        cout << "Address: " << (address.empty() ? "N/A" : address) << endl;
        cout << "Loyalty Points: " << loyaltyPoints << endl;
        cout << "Cart Items: " << cart.size() << endl;
    }
};

class Guest {
private:
    string sessionId;
public:
    Guest() {
        sessionId = "G" + to_string(rand() % 10000 + 1000);
        cout << "Guest session started: " << sessionId << endl;
    }
    
    void browseProducts(const vector<Product>& products) const {
        cout << "\n--- Product Catalog (Guest) ---\n";
        for (const auto& p : products)
            if (p.getIsActive()) p.displayInfo();
    }
    
    void searchProduct(const vector<Product>& products, string name) const {
        for (const auto& p : products)
            if (p.getName() == name) { p.displayInfo(); return; }
        cout << "No product found.\n";
    }
    
    Customer registerAsCustomer(string name, string email, string phone, string password) {
        Customer newCustomer;
        newCustomer.registerAccount(name, email, phone, password, "customer");
        cout << "Welcome " << name << "! You are now a registered customer.\n";
        return newCustomer;
    }
    
    void displayInfo() const {
        cout << "Guest Session ID: " << sessionId << endl;
    }
};

class Admin : public User {
private:
    vector<string>   promotion;
public:
    Admin() {
        role = "admin";
        status = "active";
    }

    bool registerAccount(string _name, string _email, string _phone, string _password, string _role) override {
        userId = "A" + to_string(rand() % 10000 + 1000);
        name = _name;
        email = _email;
        phone = _phone;
        passwordHash = _password;
        role = "admin";
        status = "active";
        createdAt = time(nullptr);
        lastLoginAt = createdAt;
        cout << "Registered successfully! UserID: " << userId << endl;
        return true;
    }

    void login(string identifier, string password) override {
        if ((identifier == email || identifier == userId) && password == passwordHash) {
            lastLoginAt = time(nullptr);
            cout << "[ADMIN] Login successful! Welcome, " << name << endl;
        } else {
            cout << "[ADMIN] Invalid login credentials.\n";
        }
    }

    void createProduct(vector<Product>& products, string _productId, string _name, double _price, bool _isActive, string _description = "", int _quantityStock = 0) {
        
        for (const auto& p : products) {
            if (p.getProductId() == _productId) {
                cout << "[ADMIN] Duplicate productId.\n";
                return;
            }
        }
        
        Product pNew(_productId, _name, _price, _isActive, _description, _quantityStock);
        products.push_back(pNew);
        cout << "[ADMIN] Product created: " << _name << "\n";
    }

    void updateProduct(vector<Product>& products, string productId, double newPrice, int newStock, bool newStatus, string newDesc) {
        for (auto& p : products) {
            if (p.getProductId() == productId) {
                if (newPrice > 0) p.setPrice(newPrice);
                if (newStock >= 0) p.setQuantityStock(newStock);
                if (!newDesc.empty()) p.setDescription(newDesc);
                newStatus ? p.activate() : p.deactivate();
                
                cout << "[ADMIN] Product " << productId << " updated.\n";
                return;
            }
        }
        cout << "[ADMIN] Product not found.\n";
    }

    void deleteProduct(vector<Product>& products, string productId) {
        for (auto& p : products) {
            if (p.getProductId() == productId) {
                p.deactivate();
                cout << "[ADMIN] Product " << productId << " deactivated (soft-delete).\n";
                return;
            }
        }
        cout << "[ADMIN] Product not found.\n";
    }

    void hardDeleteProduct(vector<Product>& products, string productId) {
        for (auto it = products.begin(); it != products.end(); ++it) {
            if (it->getProductId() == productId) {
                products.erase(it);
                cout << "[ADMIN] Product " << productId << " permanently deleted.\n";
                return;
            }
        }
        cout << "[ADMIN] Product not found.\n";
    }
    
    void confirmOrder(vector<Order>& allOrders, string orderId) {
        for (auto& order : allOrders) {
            if (order.orderId == orderId && order.status == "pending") {
                order.status = "confirmed";
                order.updatedAt = time(nullptr);
                cout << "[ADMIN] Order " << orderId << " confirmed.\n";
                return;
            }
        }
        cout << "[ADMIN] Could not confirm order " << orderId << " (Not found or not pending).\n";
    }

    void managePromotions(string promo) {
        promotion.push_back(promo);
        cout << "Promotion added: " << promo << "\n";
    }
    
    void viewAllProducts(const vector<Product>& products) const {
        cout << "\n--- All Products (Admin View) ---\n";
        for (const auto& p : products) {
            p.displayInfo();
        }
        cout << '\n';
    }
    
    void displayInfo() const override {
        cout << "Infomation of admin: \n";
        User::displayInfo();
        cout << "Active Promotions: " << promotion.size() << endl;
    }
};

class Staff : public User {
private:
    string currentOrderId;
    string workStatus;

public:
    Staff() {
        role = "staff";
        currentOrderId = "";
        workStatus = "idle";
        status = "active";
    }

    bool registerAccount(string _name, string _email, string _phone, string _password, string _role) override {
        userId = "S" + to_string(rand() % 1000 + 100);
        name = _name;
        email = _email;
        phone = _phone;
        passwordHash = _password;
        role = "staff";
        status = "active";
        createdAt = time(nullptr);
        lastLoginAt = createdAt;
        cout << "Registered successfully! UserID: " << userId << endl;
        return true;
    }

    void login(string identifier, string password) override {
        if ((identifier == email || identifier == userId) && password == passwordHash) {
            lastLoginAt = time(nullptr);
            cout << "[STAFF] Login successful! Welcome, " << name << endl;
        } else {
            cout << "[STAFF] Invalid login credentials.\n";
        }
    }
    
    void displayInfo() const override {
        cout << "Infomation of staff: \n";
        User::displayInfo();
        cout << "Work Status: " << workStatus << endl;
        cout << "Handling Order: " << (currentOrderId.empty() ? "None" : currentOrderId) << endl;
    }

    void claimOrder(vector<Order>& orders, string orderId) {
        if(workStatus == "working") {
            cout << "[STAFF] Cannot claim new order, already working on " << currentOrderId << endl;
            return;
        }
        for (auto& order : orders) {
            if (order.orderId == orderId) {
                if (order.status != "confirmed") {
                    cout << "[STAFF] Order must be 'confirmed' to claim. Current status: " << order.status << "\n";
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

    void updateOrderStatus(vector<Order>& orders, string newStatus) {
        if (currentOrderId.empty()) {
            cout << "[STAFF] No current order claimed.\n";
            return;
        }
        
        for (auto& order : orders) {
            if (order.orderId == currentOrderId) {
                string old = order.status;

                if (newStatus == "completed") {
                    cout << "[STAFF] Use markOrderCompleted() after 'finished'.\n";
                    return;
                }

                if (old == "confirmed" && newStatus == "in_progress") {
                    order.status = "in_progress";
                    order.updatedAt = time(nullptr);
                    cout << "[STAFF] Order " << currentOrderId << " -> in_progress\n";
                    return;
                }
                if (old == "in_progress" && newStatus == "finished") {
                    order.status = "finished";
                    order.updatedAt = time(nullptr);
                    cout << "[STAFF] Order " << currentOrderId << " -> finished\n";
                    return;
                }

                cout << "[STAFF] Invalid status transition: " << old << " -> " << newStatus << "\n";
                return;
            }
        }
        cout << "[STAFF] Current order not found.\n";
    }

    void markOrderCompleted(vector<Order>& orders) {
        if (currentOrderId.empty()) {
            cout << "[STAFF] No current order claimed.\n";
            return;
        }
        for (auto& order : orders) {
            if (order.orderId == currentOrderId) {
                if (order.status == "finished") {
                    order.status = "completed";
                    order.updatedAt = time(nullptr);
                    cout << "[STAFF] Order " << currentOrderId << " marked as completed.\n";
                    
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

    void viewWorkStatus() const {
        cout << "[STAFF] Work: " << workStatus;
        if (!currentOrderId.empty()) cout << " | Current order: " << currentOrderId;
        cout << "\n";
    }
};



//                    PAYMENT GATEWAY SYSTEM
class PaymentBase {
protected:  
    string paymentId;       // Mã thanh toán
    double amount;          // Số tiền
    string currency;        
    time_t createdAt;       // Thời gian tạo
    
public:
    
    PaymentBase() : paymentId(""), amount(0.0), currency("VND"), createdAt(0) {}
    
    PaymentBase(string id, double amt) 
        : paymentId(id), amount(amt), currency("VND") {
        createdAt = time(nullptr);
    }
    virtual void displayInfo() const {
        cout << "Payment ID: " << paymentId << endl;
        cout << "Amount: " << amount << " " << currency << endl;
    }
    
    // Getters
    string getPaymentId() const { return paymentId; }
    double getAmount() const { return amount; }
    string getCurrency() const { return currency; }
    virtual ~PaymentBase() {}
};
   //PaymentTransaction
class PaymentTransaction : public PaymentBase {
private:
    // Attributes riêng của PaymentTransaction
    // paymentId, amount, currency đã kế thừa từ PaymentBase
    string txId;                    
    string gateway;                 
    string requestPayloadHash;      
    string responseCode;            
    string responseMsg;             // response message from the gateway
    time_t tokenExpiry;             // token expiration time
    string signature;               // digital signature for validation
    
    //Simple encryption 
    string encryptData(string data) {
        string encrypted = data;
        for (size_t i = 0; i < encrypted.length(); i++) {
            encrypted[i] = encrypted[i] ^ 42; 
        }
        return encrypted;
    }
    // Helper method: Generate hash
    string generateHash(string data) {
        int hash = 0;
        for (char c : data) {
            hash = (hash * 31 + c) % 1000000;
        }
        return "HASH_" + to_string(hash);
    }

public:
    // Constructor - gọi constructor của base class (PaymentBase)
    PaymentTransaction(string _paymentId, double _amount, string _gateway) 
        : PaymentBase(_paymentId, _amount) {  // Inheritance: gọi constructor cha
        txId = "TX" + to_string(rand() % 100000);
        gateway = _gateway;
        responseCode = "000"; 
        responseMsg = "Transaction initiated";
        tokenExpiry = time(nullptr) + 3600; 
        signature = "";
    }
    
    // Method 1: signRequest() – generate digital signature for request
    void signRequest() {
        string data = txId + paymentId + gateway;
        signature = generateHash(data);
        
        // BR: All transactions must be encrypted
        requestPayloadHash = encryptData(data);
        
        cout << "Request signed with signature: " << signature << endl;
        cout << "BR: Request payload encrypted" << endl;
    }
    
    // Method 2: validateSignature() – verify response signature
    bool validateSignature(string receivedSignature) {
        if (receivedSignature == signature) {
            cout << "Signature validated successfully" << endl;
            return true;
        } else {
            cout << "Signature validation failed!" << endl;
            return false;
        }
    }
    
    // Method 3: isTokenValid() – check token validity
    bool isTokenValid() {
        time_t now = time(nullptr);
        if (now <= tokenExpiry) {
            cout << "Token is valid" << endl;
            return true;
        } else {
            cout << "Token has expired!" << endl;
            return false;
        }
    }
    
    // Getters
    string getTxId() const { return txId; }
    string getResponseCode() const { return responseCode; }
    string getResponseMsg() const { return responseMsg; }
    string getSignature() const { return signature; }
    
    // Setters
    void setResponseCode(string code) { responseCode = code; }
    void setResponseMsg(string msg) { responseMsg = msg; }
    
    // Override displayInfo() từ base class
    void displayInfo() const override {
        PaymentBase::displayInfo(); // Gọi method của class cha
        cout << "[Thông tin riêng của PaymentTransaction]:" << endl;
        cout << "TX ID: " << txId << endl;
        cout << "Gateway: " << gateway << endl;
    }
    
    // Display transaction details
    void displayTransaction() const {
        cout << "\nTransaction Details" << endl;
        cout << "TX ID: " << txId << endl;
        cout << "Payment ID: " << paymentId <<endl;
        cout << "Gateway: " << gateway << endl;
        cout << "Amount: " << amount << " " << currency <<endl;
        cout << "Response: " << responseCode << " - " << responseMsg << endl;
        cout << "Signature: " << signature << endl;
    }
};
   //Payment - Thanh toán cho đơn hàng (card, wallet, or cash)
   //KẾ THỪA từ PaymentBase (Inheritance)
class Payment : public PaymentBase {
private:
    // Attributes riêng của Payment (thêm vào kế thừa)
    // paymentId, amount, currency, createdAt đã kế thừa từ PaymentBase
    string orderId;            // linked order identifier
    string method;             // payment method (card, eWallet, cash)
    string status;             // payment status (pending, success, failed)
    time_t initiatedAt;        // payment initiation time
    time_t confirmedAt;        // payment confirmation time
    
    PaymentTransaction* transaction; // Pointer to transaction (relationship)

public:
    Payment() : PaymentBase() {
        orderId = "";
        method = "";
        status = "pending";
        initiatedAt = 0;
        confirmedAt = 0;
        transaction = nullptr;
    }
    
    
    // Method 1: initiate(orderId, amount, method) – initiate payment
    void initiate(string _orderId, double _amount, string _method) {
        paymentId = "PAY" + to_string(rand() % 100000);
        orderId = _orderId;
        amount = _amount;
        method = _method;
        status = "pending";
        initiatedAt = time(nullptr);
        confirmedAt = 0;
        
        cout << "Payment Initiated"<< endl;
        cout << "Payment ID: " << paymentId << endl;
        cout << "Order ID: " << orderId << endl;
        cout << "Amount: " << amount << " " << currency << endl;
        cout << "Method: " << method << endl;
        cout << "Status: " << status << endl;
    }
    
    // Method 2: confirm(gatewayResponse) – confirm payment from gateway
    bool confirm(PaymentTransaction& txn) {
        cout << "Confirming Payment" << endl;
        
        // Check token validity
        if (!txn.isTokenValid()) {
            status = "failed";
            cout << "Payment failed: Token expired" << endl;
            return false;
        }
        
        // Validate signature
        if (!txn.validateSignature(txn.getSignature())) {
            status = "failed";
            cout << "Payment failed: Invalid signature" << endl;
            return false;
        }
        
        // Check response code from gateway
        if (txn.getResponseCode() == "00" || txn.getResponseCode() == "000") {
            status = "success";
            confirmedAt = time(nullptr);
            transaction = &txn;
            cout << "Payment confirmed successfully!" << endl;
            return true;
        } else {
            status = "failed";
            cout << "Payment failed: " << txn.getResponseMsg() << endl;
            return false;
        }
    }
    
    // Method 3: refund(amount, reason) – issue refund
    // BR: Failed or canceled payments must be refunded within 3 business days
    void refund(double refundAmount, string reason) {
        if (status != "success") {
            cout << "Cannot refund: Payment not successful" << endl;
            return;
        }
        
        if (refundAmount > amount) {
            cout << "Cannot refund: Amount exceeds payment amount" << endl;
            return;
        }
        
        cout << "Initiating Refund" << endl;
        cout << "Payment ID: " << paymentId << endl;
        cout << "Refund Amount: " << refundAmount << " " << currency << endl;
        cout << "Reason: " << reason << endl;
        cout << "BR: Refund will be processed within 3 business days" << endl;
        
        status = "refunded";
        cout << "Status updated to: refunded" << endl;
    }
    
    //displayInfo() từ base class
    void displayInfo() const override {
        cout << "\n[Kế thừa từ PaymentBase]:" << endl;
        PaymentBase::displayInfo(); // Gọi method của class cha
        cout << "[Thông tin riêng của Payment]:" << endl;
        cout << "Order ID: " << orderId << endl;
        cout << "Method: " << method << endl;
        cout << "Status: " << status << endl;
    }
    
    // Display payment details
    void displayPayment() const {
        cout << "\n=== Payment Details ===" << endl;
        cout << "Payment ID: " << paymentId << " (inherited)" << endl;
        cout << "Order ID: " << orderId << endl;
        cout << "Method: " << method << endl;
        cout << "Amount: " << amount << " " << currency << " (inherited)" << endl;
        cout << "Status: " << status << endl;
        if (initiatedAt > 0) {
            cout << "Initiated at: " << ctime(&initiatedAt);
        }
        if (confirmedAt > 0) {
            cout << "Confirmed at: " << ctime(&confirmedAt);
        }
    }
    
    // Getters
    string getPaymentId() const { return paymentId; }
    string getOrderId() const { return orderId; }
    string getMethod() const { return method; }
    double getAmount() const { return amount; }
    string getStatus() const { return status; }
};
/* ========================================================================
   Payment Gateway - Cổng thanh toán xử lý giao dịch
   FR: Connect with payment gateway to process transactions
   FR: Display payment results (success/failure)
   ======================================================================== */
class PaymentGateway {
private:
    string gatewayName;
    vector<Payment> payments;
    vector<PaymentTransaction> transactions;
    
public:
    // Constructor
    PaymentGateway(string name) : gatewayName(name) {
        cout << "Payment Gateway: " << name << endl;
        cout << "BR: All transactions will be encrypted" << endl;
        cout << "BR: 3-Day Refund Policy Active" << endl;
    }
    
    // FR: Process transaction through gateway
    Payment processTransaction(string orderId, double amount, string method) {
        cout << "PROCESSING TRANSACTION" << endl;
        
        // Create payment
        Payment payment;
        payment.initiate(orderId, amount, method);
        
        // Create transaction with gateway
        PaymentTransaction txn(payment.getPaymentId(), amount, gatewayName);
        
        // Sign request (BR: must be encrypted)
        txn.signRequest();
        
        // Simulate gateway processing
        cout << "\nConnecting to " << gatewayName << "..." << endl;
        cout << "Processing..." << endl;
        
        // Simulate response (90% success rate)
        int result = rand() % 100;
        if (result < 90) {
            txn.setResponseCode("00");
            txn.setResponseMsg("Transaction successful");
        } else {
            txn.setResponseCode("05");
            txn.setResponseMsg("Transaction failed");
        }
        
        // Confirm payment
        bool success = payment.confirm(txn);
        
        // Store records
        payments.push_back(payment);
        transactions.push_back(txn);
        
        // FR: Display payment results
        displayPaymentResult(payment, txn, success);
        
        // BR: Auto-refund if failed
        if (!success && method != "Cash") {
            payment.refund(amount, "Transaction failed - auto refund");
        }
        
        return payment;
    }
    
    // FR: Display payment results (success/failure)
    void displayPaymentResult(const Payment& payment, const PaymentTransaction& txn, bool success) {
        cout << "PAYMENT RESULT" << endl;
        if (success) {
            cout << "PAYMENT SUCCESSFUL" << endl;
        } else {
            cout << "PAYMENT FAILED" << endl;
        }
        payment.displayPayment();
        txn.displayTransaction();
    }
    
    // List all payments
    void listAllPayments() const {
        if (payments.empty()) {
            cout << "\nNo payments found." << endl;
            return;
        }
        cout << "PAYMENT HISTORY" << endl;
        for (size_t i = 0; i < payments.size(); i++) {
            cout << "\n[Payment #" << (i+1) << "]";
            payments[i].displayPayment();
        }
    }
};

//                    DELIVERY SYSTEM
class DeliveryBase {
protected:  
    string deliveryId;      // Mã giao hàng
    string orderId;         // Mã đơn hàng
    string status;          // Trạng thái giao hàng
    time_t createdAt;       // Thời gian tạo
    
public:
    DeliveryBase() : deliveryId(""), orderId(""), status("pending"), createdAt(0) {}
    DeliveryBase(string delId, string ordId) 
        : deliveryId(delId), orderId(ordId), status("pending") {
        createdAt = time(nullptr);
    }
    
    virtual void displayInfo() const {
        cout << "\n--- Delivery Information ---" << endl;
        cout << "Delivery ID: " << deliveryId << endl;
        cout << "Order ID: " << orderId << endl;
        cout << "Status: " << status << endl;
    }
    
    // Getters
    string getDeliveryId() const { return deliveryId; }
    string getOrderId() const { return orderId; }
    string getStatus() const { return status; }
    
    // Virtual destructor cho base class
    virtual ~DeliveryBase() {}
};

   //KẾ THỪA từ DeliveryBase (Inheritance)
class Delivery : public DeliveryBase {
private:
    
    // deliveryId, orderId, status, createdAt đã kế thừa từ DeliveryBase
    string courierPartnerId;        // BR15: one courier at a time
    // status đã kế thừa từ DeliveryBase
    time_t assignedAt;              
    time_t confirmedAt;             // BR16: within 5 minutes
    bool isConfirmedWithin5Min;
    
public:
    // Constructor - Gọi constructor của base class (Constructor chaining)
    Delivery(string _orderId) 
        : DeliveryBase("DEL" + _orderId.substr(3), _orderId),  // Gọi base constructor
          isConfirmedWithin5Min(false), courierPartnerId(""), assignedAt(0), confirmedAt(0) {
        // deliveryId, orderId, status, createdAt đã được khởi tạo ở base class
        cout << "Delivery created: " << deliveryId << endl;
    }
    
    // BR15: Assign delivery to only one courier at a time
    bool assignToCourier(string courierId) {
        if (!courierPartnerId.empty()) {
            cout << "Delivery already assigned to courier: " << courierPartnerId << endl;
            cout << "Each delivery can only be assigned to ONE courier at a time.\n";
            return false;
        }
        courierPartnerId = courierId;
        status = "assigned";
        assignedAt = time(0);
        cout << "Delivery " << deliveryId << " assigned to Courier: " << courierId << endl;
        cout << "Courier must confirm within 5 minutes.\n";
        return true;
    }
    
    //Courier confirms receipt within 5 minutes
    bool confirmReceipt() {
        if (status != "assigned") {
            cout << "Cannot confirm: Delivery status is " << status << endl;
            return false;
        }
        confirmedAt = time(0);
        double timeDiff = difftime(confirmedAt, assignedAt);
        if (timeDiff <= 300) { // 5 minutes
            isConfirmedWithin5Min = true;
            status = "confirmed";
            cout << "Delivery confirmed by courier within " << timeDiff << " seconds.\n";
            cout << "Confirmed within 5 minutes.\n";
            return true;
        } else {
            isConfirmedWithin5Min = false;
            status = "confirmed_late";
            cout << "Delivery confirmed but LATE (" << timeDiff << " seconds).\n";
            cout << "Exceeded 5-minute limit!\n";
            return false;
        }
    }
    // Update delivery status to "Out for Delivery"
    void markOutForDelivery() {
        if (status == "confirmed" || status == "confirmed_late") {
            status = "out_for_delivery";
            cout << "Status updated: OUT FOR DELIVERY\n";
            cout << "Courier " << courierPartnerId << " is on the way to customer.\n";
        } else {
            cout << "Cannot mark out for delivery. Current status: " << status << endl;
        }
    }
    // Update delivery status to "Delivered"
    void markDelivered() {
        if (status == "out_for_delivery") {
            status = "delivered";
            cout << "Status updated: DELIVERED\n";
            cout << "  Delivery " << deliveryId << " completed successfully!\n";
        } else {
            cout << "Cannot mark delivered. Current status: " << status << endl;
        }
    }
    
    // Mark delivery as failed
    void markFailed(string reason) {
        status = "failed";
        cout << "Delivery FAILED: " << reason << endl;
    }
    
    // Customer tracks delivery status in real-time
    void trackStatus() const {
        cout << "DELIVERY TRACKING\n";
        cout << "Delivery ID: " << deliveryId << endl;
        cout << "Order ID: " << orderId << endl;
        cout << "Status: " << status << endl;
        cout << "Courier: " << (courierPartnerId.empty() ? "Not assigned" : courierPartnerId) << endl;
        if (status == "assigned") {
            cout << "Waiting for courier confirmation...\n";
        } else if (status == "confirmed") {
            cout << "Courier confirmed. Preparing...\n";
        } else if (status == "out_for_delivery") {
            cout << "Out for delivery!\n";
        } else if (status == "delivered") {
            cout << "DELIVERED!\n";
        }
    }
    
    // OVERRIDE method từ base class - Thể hiện tính đa hình (Polymorphism)
    void displayInfo() const override {
        cout << "\n--- Delivery Details ---" << endl;
        cout << "Delivery ID: " << deliveryId << endl;
        cout << "Order ID: " << orderId << endl;
        cout << "Status: " << status << endl;
        cout << "Courier Partner: " << (courierPartnerId.empty() ? "Not assigned" : courierPartnerId) << endl;
        cout << "Confirmed within 5 min: " << (isConfirmedWithin5Min ? "Yes" : "No") << endl;
    }
    
    // Getters (một số getter đã có ở base class)
    string getCourierPartnerId() const { return courierPartnerId; }
    bool getIsConfirmedWithin5Min() const { return isConfirmedWithin5Min; }
};
  
   //KẾ THỪA từ DeliveryBase
class DeliveryPartner : public DeliveryBase {
private:
    // deliveryId, orderId, status, createdAt đã kế thừa từ DeliveryBase
    string partnerId;
    string name;
    bool isAvailable;
    
public:
    // Constructor - Gọi constructor của base class
    DeliveryPartner(string _partnerId, string _name)
        : DeliveryBase(_partnerId + "_DEL", ""),  // Gọi base constructor
          partnerId(_partnerId), name(_name), isAvailable(true) {
        // deliveryId được tạo từ partnerId
        status = "available";  // Status cho courier
        cout << "Courier registered: " << name << endl;
    }
    // Accept a delivery assignment
    bool acceptDelivery(Delivery& delivery) {
        if (!isAvailable) {
            cout << "Courier not available.\n";
            return false;
        }
        if (delivery.assignToCourier(partnerId)) {
            cout << "Courier " << name << " accepted delivery\n";
            return true;
        }
        return false;
    }
    
    //BR16: Confirm order receipt within 5 minutes
    bool confirmOrderReceipt(Delivery& delivery) {
        if (delivery.confirmReceipt()) {
            cout << "Order confirmed by " << name << endl;
            return true;
        } else {
            cout << "Confirmation recorded but exceeded time limit.\n";
            return false;
        }
    }
    // Update delivery status to "Out for Delivery"
    void updateStatusOutForDelivery(Delivery& delivery) {
        delivery.markOutForDelivery();
    }
    // Update delivery status to "Delivered"
    void updateStatusDelivered(Delivery& delivery) {
        cout << "Courier: " << name << " completing delivery\n";
        delivery.markDelivered();
    }

    // OVERRIDE method từ base class - Thể hiện tính đa hình (Polymorphism)
    void displayInfo() const override {
        cout << "\n--- Courier Partner Info ---" << endl;
        cout << "Partner ID: " << partnerId << endl;
        cout << "Name: " << name << endl;
        cout << "Status: " << status << endl;
        cout << "Available: " << (isAvailable ? "YES" : "NO") << endl;
    }
    
    // Getters
    string getPartnerId() const { return partnerId; }
    string getName() const { return name; }
    bool getIsAvailable() const { return isAvailable; }
    void setAvailable(bool status) { isAvailable = status; }
};


int main() {
    return 0;
}
