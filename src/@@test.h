class TestClass {
public:
    TestClass()     {
        puts("constructed.");
    }


    void ~TestClass()     {
        puts("destructed.");
    }


    void touch()     {
        puts("don't touch me!");
    }


};
void func() {
    TestClass A;
    A.touch();
}

;
