#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
using namespace std;

class Shop
{
private:
    char* name;
    float* revenues;
    int     revenueCount;

    bool    isOpen;
    double  expenses;

    const int foundingYear;
    const int id;

    static int currentYear;
    static int counter;

public:

    void display()
    {
        cout << "Name     : " << this->name << endl;
        cout << "Founded  : " << this->foundingYear << endl;
        cout << "Expenses : " << this->expenses << endl;
    }

    // A constructor builds (declares + initializes) an object.
    // No return type. Name = class name.
    // const and static fields must be set in the initializer list (:).
    //
    // Usage:
    //   Shop a;                                         — default
    //   Shop b("Kaufland", 1997, true, 400.0, 3, arr); — full
    //   Shop c("Penny", 250.0);                         — partial

    // Default constructor
    Shop() : foundingYear(2004), id(Shop::counter++)
    {
        this->expenses = 0;
        this->isOpen = false;
        this->name = new char[strlen("na") + 1];
        strcpy(this->name, "na");
        this->revenueCount = 0;
        this->revenues = NULL;
    }

    // Full constructor
    Shop(const char* nameIn, int yearIn, bool stateIn,
        double expIn, int revCountIn, float* revsIn)
        : foundingYear(yearIn), id(Shop::counter++)
    {
        this->expenses = (expIn >= 0) ? expIn : 0;
        this->isOpen = stateIn;

        if (strlen(nameIn) > 2) {
            this->name = new char[strlen(nameIn) + 1];
            strcpy(this->name, nameIn);
        }
        else {
            this->name = new char[strlen("na") + 1];
            strcpy(this->name, "na");
        }

        if (revCountIn > 0 && revsIn != NULL) {
            this->revenueCount = revCountIn;
            this->revenues = new float[revCountIn];
            for (int i = 0; i < revCountIn; i++)
                this->revenues[i] = (revsIn[i] >= 0) ? revsIn[i] : 0;
        }
        else {
            this->revenueCount = 0;
            this->revenues = NULL;
        }
    }

    // Partial constructor
    Shop(const char* nameIn, double expIn)
        : foundingYear(2004), id(Shop::counter++)
    {
        this->isOpen = false;
        this->expenses = (expIn >= 0) ? expIn : 0;
        this->revenueCount = 0;
        this->revenues = NULL;

        if (strlen(nameIn) > 2) {
            this->name = new char[strlen(nameIn) + 1];
            strcpy(this->name, nameIn);
        }
        else {
            this->name = new char[strlen("na") + 1];
            strcpy(this->name, "na");
        }
    }

    // Destructor — called automatically when the object goes out of scope
    // or when delete is used on a pointer to the object.
    // Releases heap memory to prevent leaks.
    ~Shop()
    {
        Shop::counter--;
        delete[] this->name;
        delete[] this->revenues;
    }

    // Copy constructor — creates a new object initialized from an existing one.
    // Deep copy: we manually duplicate heap-allocated memory.
    // Without this, the compiler does a shallow copy — both objects
    // share the same pointer, leading to double-free bugs.
    //
    // Usage: Shop b(a);
    Shop(const Shop& s)
        : foundingYear(s.foundingYear), id(Shop::counter++)
    {
        this->isOpen = s.isOpen;
        this->expenses = s.expenses;
        this->revenueCount = s.revenueCount;

        this->name = new char[strlen(s.name) + 1];
        strcpy(this->name, s.name);

        this->revenues = new float[s.revenueCount];
        for (int i = 0; i < s.revenueCount; i++)
            this->revenues[i] = s.revenues[i];
    }

    // ---------- OPERATORS ----------

    // Assignment operator — assigns the values of one existing object to another.
    // Unlike the copy constructor, both objects already exist.
    // Self-assignment guard (this != &s) prevents deleting our own memory.
    // Returns *this to allow chaining: a = b = c;
    Shop& operator=(const Shop& s)
    {
        if (this != &s) {
            delete[] this->name;
            delete[] this->revenues;

            this->isOpen = s.isOpen;
            this->expenses = s.expenses;
            this->revenueCount = s.revenueCount;

            this->name = new char[strlen(s.name) + 1];
            strcpy(this->name, s.name);

            this->revenues = new float[s.revenueCount];
            for (int i = 0; i < s.revenueCount; i++)
                this->revenues[i] = s.revenues[i];
        }
        return *this;
    }

    // ---------- GETTERS ----------

    // For dynamic fields (char*), return a copy to protect internal memory.
    double getExpenses() { return this->expenses; }
    bool   getIsOpen() { return this->isOpen; }
    int    getFoundingYear() { return this->foundingYear; }

    char* getName()
    {
        char* copy = new char[strlen(this->name) + 1];
        strcpy(copy, this->name);
        return copy;
    }

    char* getNameEasy() { return this->name; }

    // Static getter — called on the class, not on an instance. No 'this' pointer.
    // Usage: Shop::getCurrentYear();
    static int getCurrentYear() { return Shop::currentYear; }

    // ---------- SETTERS ----------

    void setName(const char* nameIn)
    {
        if (strlen(nameIn) > 2) {
            delete[] this->name;
            this->name = new char[strlen(nameIn) + 1];
            strcpy(this->name, nameIn);
        }
    }

    void setRevenues(int count, float* arr)
    {
        if (count > 0 && arr != NULL) {
            delete[] this->revenues;
            this->revenueCount = count;
            this->revenues = new float[count];
            for (int i = 0; i < count; i++)
                this->revenues[i] = (arr[i] >= 0) ? arr[i] : 0;
        }
    }

    void setExpenses(double val) { if (val >= 0) this->expenses = val; }
    void setIsOpen(bool val) { this->isOpen = val; }

    // Static setter — validates before updating the shared static field.
    static void setCurrentYear(int yr)
    {
        if (yr >= 2000) Shop::currentYear = yr;
    }

    // ---------- ARITHMETIC OPERATORS ----------

    // Compound assignment (+=): modifies 'this', returns a reference to it.
    // Usage: a += 200.0f;
    Shop& operator+=(float val)
    {
        this->expenses += val;
        return *this;
    }

    // Binary arithmetic (+): does NOT modify 'this'.
    // Makes a copy, modifies the copy, returns it.
    // Requires a working copy constructor.
    // Usage: Shop b = a + 200.0f;
    Shop operator+(float val)
    {
        Shop s(*this);
        s.expenses += val;
        return s;
    }

    // ---------- INCREMENT / DECREMENT ----------

    // Pre-increment (++a): modifies 'this', returns a reference to the modified object.
    // Usage: ++a;  — expenses increase by 100, then the result is used.
    Shop& operator++()
    {
        this->expenses += 100;
        return *this;
    }

    // Post-increment (a++): saves a copy BEFORE modifying 'this', returns the copy.
    // The dummy int param distinguishes it from pre-increment — it has no value.
    // Usage: a++;  — result is the old value, 'this' gets modified.
    Shop operator++(int)
    {
        Shop s(*this);
        this->expenses += 100;
        return s;
    }

    // ---------- RELATIONAL OPERATORS ----------

    // Returns bool based on comparing a field to a given value.
    // Usage: if (a >= 500.0f) { ... }
    bool operator>=(float val)
    {
        return this->expenses >= val;
    }

    // ---------- SPECIAL OPERATORS ----------

    // operator[]: index into the revenues array.
    // Throws if index is out of bounds.
    // Usage: float r = a[1];
    float operator[](int index)
    {
        if (index >= 0 && index < this->revenueCount)
            return this->revenues[index];
        throw new exception();
    }

    // operator(): custom behavior — here it increases expenses.
    // Usage: a(200.0f);
    void operator()(float val)
    {
        this->expenses += val;
    }

    // Cast operator: explicit conversion to float — returns expenses.
    // 'explicit' prevents unintended implicit conversions.
    // Usage: float x = (float)a;
    explicit operator float()
    {
        return (float)this->expenses;
    }

    // operator!: logical negation of 'isOpen'.
    // Usage: if (!a) { cout << "Store is closed"; }
    bool operator!()
    {
        return !this->isOpen;
    }

    // operator~: bitwise NOT repurposed — toggles isOpen.
    // Only receives the object it was called on, no extra params.
    // Usage: ~a;
    void operator~()
    {
        this->isOpen = !this->isOpen;
    }

    // ---------- STREAM OPERATORS ----------

    // Declared as friend so they can access private members.
    friend ostream& operator<<(ostream& out, Shop s);
    friend istream& operator>>(istream& in, Shop& s);
};

int Shop::currentYear = 2025;
int Shop::counter = 0;

// ostream<<: pass by value — works on a copy of the object.
// Returns the stream to allow chaining: cout << a << b;
ostream& operator<<(ostream& out, Shop s)
{
    out << "Name     : " << s.name << "\n";
    out << "ID       : " << s.id << "\n";
    out << "Founded  : " << s.foundingYear << "\n";
    out << "Open     : " << s.isOpen << "\n";
    out << "Expenses : " << s.expenses << "\n";
    out << "Revenues : ";
    for (int i = 0; i < s.revenueCount; i++)
        out << s.revenues[i] << " ";
    out << "\n\n";
    return out;
}

// istream>>: always pass by reference — we modify the actual object.
istream& operator>>(istream& in, Shop& s)
{
    char buffer[200];
    cout << "Name: ";          in >> buffer;      s.setName(buffer);
    cout << "Is open: ";       in >> s.isOpen;
    cout << "Expenses: ";      float e; in >> e;  s.setExpenses(e);

    int nr; float aux[200];
    cout << "Revenue count: "; in >> nr;
    for (int i = 0; i < nr; i++) {
        cout << "Revenue [" << i + 1 << "] : ";
        in >> aux[i];
    }
    s.setRevenues(nr, aux);
    return in;
}

int main()
{
    float revs[3] = { 100, 500, 300 };

    Shop a;
    Shop b("La 2 pasi", 2005, true, 400.0, 3, revs);
    Shop c("Mega", 2008, false, 500.0, 3, revs);

    Shop* p = new Shop();
    delete p;
    p = NULL;

    Shop d(b);  // copy constructor
    a = b;      // operator=

    b.setName("SRL Mega Image");

    Shop::setCurrentYear(2026);

    cout << b;
    b += 200.0f;        // operator+=
    b = b + 200.0f;     // operator+
    ++b;                // pre-increment
    b++;                // post-increment
    cout << b;

    if (b >= 200)
        cout << "Expenses exceeded 200" << endl;
    else
        cout << "Expenses are under control" << endl;

    b(200.0f);            // operator()
    cout << b;
    cout << b[2] << endl; // operator[]

    float x = (float)b;   // explicit cast
    ~b;                   // toggle isOpen
    cout << (!b);         // operator!

    return 0;
}