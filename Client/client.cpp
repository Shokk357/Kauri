#include <iostream>
#include "../ASN/ASN.h"

int main() {
    ASN<std::vector<int>> a;
    std::vector<int> b = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
    std::vector<uint8_t> c;
    a.handle(b, c);
//   auto elen = a.decode(c.data());
    a.snd(c);

    return 0;
}
