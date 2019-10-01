//<Define an enum called StatusEnus with the enumerations "INVALID", "VALID" and "CONSUMED">
enum StatusEnum {INVALID, VALID, CONSUMED};
//<Define a typedef structure with the enum above and an "int" variable called "data">
typedef struct
  {

    enum StatusEnum status;
    int data;
  }ShmData;
