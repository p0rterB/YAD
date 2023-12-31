#include <iostream>
#include <sstream>
#include <algorithm>
#include <iomanip>

#include "defs.h"

int processData(unsigned char *key_basis, unsigned char *data_id, unsigned char *dst);
unsigned long int process(char a1, unsigned int a2, unsigned int a3, int a4, int a5);

template <typename A>
unsigned char _bittest(A *a, int b)
{
    auto const bits{reinterpret_cast<unsigned char const *>(a)};
    auto const value{bits[b >> 3]};
    auto const mask{(unsigned char)(1 << (b & 7))};
    return (value & mask) != 0;
}

static int char2int(char input)
{
    if (input >= '0' && input <= '9')
        return input - '0';
    if (input >= 'A' && input <= 'F')
        return input - 'A' + 10;
    if (input >= 'a' && input <= 'f')
        return input - 'a' + 10;
    throw std::invalid_argument("Invalid input string");
}

static void hex2bin(const char *src, char *target)
{
    while (*src && src[1])
    {
        *(target++) = char2int(*src) * 16 + char2int(src[1]);
        src += 2;
    }
}

//linked data for [0x01,0xE1,0x32,0xCA,0xE5,0x27,0xBD,0x21,0x62,0x0E,0x82,0x2F,0x58,0x51,0x49,0x32]
static const uint32_t playIntentKey[768] = 
{
    0x0BC5821DB, 0x0CE131409, 0x47D1599E, 0x616328D5, 0x0A19D78EE,
    0x0D185AFEA, 0x34ED8DB9, 0x8CAAAFE5, 0x0CF77E218, 0x510D04B5,
    0x8A9CE213, 0x0C12F8BCA, 0x0FABF6927, 0x747DAEBC, 0x8E0F7DF3,
    0x9FCA7022, 0x215F92BF, 0x40FF373B, 0x0EA694507, 0x0FC243736,
    0x7DB959D3, 0x8F744C02, 0x9329196, 0x0FF96D317, 0x3926B074,
    0x0BABBD311, 0x0FD890544, 0x95748FA7, 0x0B8AE053F, 0x3A4327DC,
    0x28D08C54, 0x0A28ED1E9, 0x0BC20A120, 0x0CDDB934F, 0x96D95DB5,
    0x0BA12D34E, 0x6FBE4CA, 0x2A355A63, 0x0ABCA7D00, 0x0BD856F2F,
    0x0F7154C8C, 0x78AA6F29, 0x6737D3A1, 0x30359E08, 0x2B9A2871,
    0x0C385B2D3, 0x0BEEA3D3D, 0x7EAAB2CE, 0x7A0F3D38, 0x689CA1B0,
    0x0EA31C44D, 0x0FBECB67B, 0x0C4EA80E2, 0x0FE7A5E40, 0x800F80DD,
    0x58467D90, 0x0F03207F2, 0x0EB96925B, 0x0FD51848A, 0x0A6BB9256,
    0x0B8768485, 0x81744EEB, 0x7001B363, 0x0F196D600, 0x351C82F,
    0x44B88295, 0x0A8206065, 0x0D8089761, 0x1842E77A, 0x793D5E7A,
    0x0F2FBA40F, 0x2C8B816C, 0x0AE20A40A, 0x0BFDB9638, 0x0B63FD7B8,
    0x19A7B588, 0x498FEC84, 0x0ACF7CA54, 0x4C1D9448, 0x0AF857218,
    0x0C917414F, 0x1FA7F92D, 0x3162EB5C, 0x72C9A5C2, 0x0EC87EB56,
    0x619BA8E, 0x7FD80022, 0x0BDA4E96C, 0x210CC73B, 0x78C9E967,
    0x742E73D0, 0x0EDECB965, 0x77E889C, 0x5E0F407A, 0x77A10FB1,
    0x0F15F5546, 0x7A2EB775, 0x0DD969545, 0x0D7ECC41, 0x30B841DA,
    0x5F740E88, 0x0A0DAC8EF, 0x1A990E83, 0x342ADDBA, 0x0DEFB6353,
    0x36B8857F, 0x0E71A9702, 0x0A6DB0C93, 0x0A23F96FD, 0x3A2B215F,
    0x358FABC9, 0x0AF4DF15D, 0x0A5B232DE, 0x6EAFFD44, 0x0A83FDAA2,
    0x0B9FACCD1, 0x3B8FEF6E, 0x5EC96507, 0x0F6B4EF69, 0x0CEEBEC1B,
    0x48AA31B0, 0x623C00E7, 0x2B39CB4E, 0x3CF4BD7C, 0x76849ADA,
    0x0AD174491, 0x0D050BA2A, 0x683C448C, 0x63A0CEF6, 0x0DD5F148A,
    0x1EC5CEF1, 0x77E968E9, 0x0D650FDCF, 0x0E80BEFFD, 0x69A1129B,
    0x7B5C04C9, 0x24C61295, 0x368104C4, 0x94E899AA, 0x0D7B5CBDD,
    0x0E970BE0C, 0x6B05E0A9, 0x0A495BE07, 0x262AE0A4, 0x14B8451C,
    0x964D67B9, 0x0A80859E7, 0x299D7C85, 0x943F99E7, 0x0C427D0E3,
    0x278FAEB2, 0x161D132A, 0x97B235C7, 0x0A96D27F6, 0x2B024A93,
    0x649227F1, 0x5FF6B25B, 0x28F47CC1, 0x35AF2606, 0x991703D6,
    0x0D2A6E134, 0x543C03D1, 0x65F6F5FF, 0x0C45E8AE6, 0x0D6197D14,
    0x0EFAB4C4B, 0x0C250BD13, 0x0F238F410, 0x55A0D1DF, 0x675BC40E,
    0x0C5C358F4, 0x0D77E4B23, 0x59136DC0, 0x92A34B1E, 0x0F39DC21E,
    0x6D5C07B3, 0x63C04933, 0x0C7282703, 0x1EE5492E, 0x824D26FD,
    0x9408192C, 0x0AD99E863, 0x42AA041, 0x65251742, 0x6B84805,
    0x204A173C, 0x83B1F50C, 0x0B39A2C08, 0x170209D8, 0x58F6E50,
    0x46F628B6, 0x0C0B46E4A, 0x21AEE54B, 0x9B6D2AE0, 0x0DCD3E546,
    0x0B8FB1F4, 0x4CF66C5B, 0x5EB15E89, 0x0C2193C59, 0x0DBAB0B90,
    0x0A4A8D5F7, 0x0DE38B354, 0x0CF48003, 0x4E5B3A69, 0x0C8197FFE,
    0x0E1AB4F35, 0x1B3B2C93, 0x9CD04F30, 0x8B5DB3A8, 0x545B7E0E,
    0x4FC00878, 0x0D1552B15, 0x0E3101D44, 0x84A34E07, 0x7B078F87,
    0x8CC281B6, 0x0E57A453, 0x20129682, 0x0E91060E8, 0x0C49D681,
    0x0A43560E3, 0x7C6C5D96, 0x8E274FC5, 0x0FBC7262, 0x494C4FC0,
    0x0CAE1725D, 0x0DC9C648B, 0x0A59A2EF2, 0x7DD12BA5, 0x15BCB607,
    0x11214070, 0x4AB11DCE, 0x0CC46406B, 0x0BAD3A4E3, 0x3C68C780,
    0x958C6178, 0x17218415, 0x3A5AF9AE, 0x0D2468410, 0x0CDAB0E7A,
    0x0BC3872F1, 0x3DCD958F, 0x4F8887BD, 0x0F8F29589, 0x52162F81,
    0x0D3AB521F, 0x0E566444D, 0x0DBCA85CE, 0x3F32639D, 0x78C240FB,
    0x0FA576398, 0x0C1255C7, 0x6A79EAAD, 0x0C39D84A5, 0x4532A742,
    0x7EC284A0, 0x985453D7, 0x1212996B, 0x0D7723D5, 0x6BDEB8BB,
    0x7D99AAEA, 0x0FF2ECD87, 0x802752AE, 0x1BC754B, 0x1377677A,
    0x54DE21E0, 0x8399EE8F, 0x0C500A8F5, 0x286886C5, 0x3A2378F3,
    0x321435A, 0x3CB120B8, 0x33156238, 0x0ACD3A7CC, 0x0C6657704,
    0x4023BC98, 0x59B58BCF, 0x0D373D164, 0x0ABAACE17, 0x74A8987D,
    0x0CC65BAA9, 0x0C7CA4512, 0x41888AA7, 0x82EF450D, 0x0B1AB11BC,
    0x0F311CC22, 0x6CD011B6, 0x68349C20, 0x31326687, 0x611A9D83,
    0x8454131C, 0x0DAE4CAF9, 0x0F4769A30, 0x6E34DFC5, 0x87C6AEFC,
    0x184F491, 0x42EBAEF7, 0x9C0F48EF, 0x0FA76DDD5, 0x0F5DB683F,
    0x6F99ADD3, 0x0B100683A, 0x2ABEADCE, 0x2122EF4F, 0x9AE134E3,
    0x0FBDBABE4, 0x0D969E12, 0x8F2BC0B0, 0x0C8BB9E0D, 0x4A50C0AA,
    0x2287BD5D, 0x9C4602F2, 0x0B5D7D229, 0x0EF67AF87, 0x0B86579ED,
    0x0CA206C1C, 0x4BB58EB9, 0x3A42F331, 0x0BBD815CE, 0x0B73CA037,
    0x58CFD0FB, 0x7261A032, 0x60EF04AA, 0x29ECCF11, 0x3BA7C13F,
    0x0BD3CE3DC, 0x0F6CCC13A, 0x7861E3D7, 0x8A1CD606, 0x0CA57261F,
    0x0E3E8F556, 0x0F5A3E784, 0x0E6769D1A, 0x0F8318F49, 0x79C6B1E6,
    0x8B81A414, 0x0E9E938FB, 0x0FBA42B29, 0x1EDDA0C2, 0x0B6C92B24,
    0x0F9965D57, 0x7B2B7FF4, 0x69B8E46C, 0x0EB4E0709, 0x24DDE467,
    0x0A6730704, 0x0B82DF933, 0x0B392839C, 0x28508047, 0x0B11FE277,
    0x1487C047, 0x2642B275, 0x0A7D7D512, 0x0B992C741, 0x3B27E9DE,
    0x29B54E56, 0x4CEEC3EF, 0x0E4DA4E51, 0x45D4C551, 0x0A93CA321,
    0x0E2CC807F, 0x195F2A36, 0x52EF0794, 0x6C80D6CB, 0x0E63F1C5F,
    0x0FFD0EB97, 0x0C0F7D8F5, 0x25E935B, 0x4298E373, 0x5453D5A2,
    0x0D5E8F83F, 0x0E7A3EA6E, 0x290AA4D4, 0x57C67183, 0x992D2BE9,
    0x5A541947, 0x73E5E87E, 0x0EDA42E13, 0x2F0AE879, 0x9272C649,
    0x81002AC0, 0x9A91F9F8, 0x14503F8C, 0x2DE20EC3, 0x0EF08FC21,
    0x306FB688, 0x0AA2DFC1C, 0x0A0923D9C, 0x0B24D2FCB, 0x15B50D9B,
    0x6D722FC6, 0x0D0DA0D96, 0x0C73E4F16, 0x88653C74, 0x0A1F70BAB,
    0x1BB55140, 0x35472077, 0x6ED6FDD5, 0x0D23EDBA4, 0x0DEF984E9,
    0x426162B9, 0x0A35BD9BA, 0x1D1A1F4E, 0x5E80D9B4, 0x703BCBE3,
    0x0F1D0EE80, 0x0E05E52F8, 0x61F37595, 0x73AE67C4, 0x0FEEB30C2,
    0x5FE5A7C3, 0x0D9A3ED57, 0x0D0082ED8, 0x0E1C32106, 0x635843A4,
    0x9CE82101, 0x1E7D439E, 0x303835CD, 0x707285E6, 0x0D16CFCE6,
    0x69588748, 0x8C91FCE1, 0x9E4CEF10, 0x1FE211AD, 0x319D03DC,
    0x900498C2, 0x0A1BF8AF0, 0x52768C0, 0x0A44D32B5, 0x9FB1BD1E,
    0x2146DFBB, 0x0FD44433, 0x916966D0, 0x0CAF9442E, 0x4C8E66CB,
    0x5E4958FA, 0x27472360, 0x60D700BE, 0x390DFD71, 0x0D0F987D3,
    0x0CC5E123D, 0x4DF334DA, 0x5FAE2708, 0x0E14349A5, 0x0CFD0AE1D,
    0x0C0A363B3, 0x0D25E55E1, 0x0EBF02519, 0x65AE6AAD, 0x88E7E046,
    0x0BF7A89FD, 0x0F90A675B, 0x7A9F89F8, 0x8C5A7C27, 0x0ED54F327,
    0x8EE823EB, 0x0A879F322, 0x0FF0AAB00, 0x0FA6F3569, 0x925ABFCB,
    0x8DBF4A35, 0x0F546CD2, 0x0FDE1D14A, 0x86B1337A, 0x6F790E,
    0x1A014845, 0x93BF8DDA, 0x0B6F90373, 0x4EE48DD5, 0x271B8A88,
    0x0BF0714EA, 0x1D4471D, 0x1B661654, 0x95245BE9, 0x0D68B164F,
    0x50495BE3, 0x46AD9D64, 0x0C06BE2F8, 0x0BBD06D62, 0x138D8F8D,
    0x0DC8B59F4, 0x0EE464C22, 0x2E809C3B, 0x48126B72, 0x0C1D0B107,
    0x0DB62803E, 0x7CF5B102, 0x8EB0A330, 0x8514E4B1, 0x4E12AF17,
    0x5FCDA146, 0x0C3357F15, 0x49C397C, 0x7E5A7F10, 0x97EC4E47,
    0x8679B2BF, 0x37F854, 0x419EB2BA, 0x0A9C7D20, 0x1C576F4F,
    0x7FBF4D1F, 0x76238E9F, 0x0EFE1D434, 0x973A36B, 0x430380C8,
    0x0C498A366, 0x1DBC3D5E, 0x9F515FFB, 0x8DDEC473, 0x0F73E710,
    0x4903C46D, 0x0AC6BA23D, 0x0C5FD7174, 0x0D7B863A3, 0x361FF889,
    0x0B7187DB0, 0x38ADA04D, 0x4A68927C, 0x0CBFDB519, 0x0DDB8A748,
    0x0B5EFA3FA, 0x4DDB2E5D, 0x7114A3F5, 0x0F2A9C692, 0x4BCD608A,
    0x0CD628328, 0x6F26085, 0x3D850A3C, 0x7714E79A, 0x72797204,
    0x0A64FC66, 0x5C986D0, 0x9DB51132, 0x0BD54B5AE, 0x66BEC37A,
    0x7879B5A9, 0x0FA0ED846, 0x0BC9CA74, 0x4559A7D2, 0x7BEC5189,
    0x9F25C722, 0x68239189, 0x79DE83B7, 0x0FB73A654, 0x350383B2,
    0x4E9552E9, 0x0A5260AC7, 0x0B6E0FCF6, 0x38761F93, 0x33DAA9FC,
    0x132EDC28, 0x366851C1, 0x6CFAFB78, 0x0A68AD8D5, 0x0C01CA80D,
    0x39DAEDA1, 0x736ACAFF, 0x0F4FFED9C, 0x0E38D5214, 0x0AC8B1C7A,
    0x0A7EFA6E4, 0x2984C981, 0x3B3FBBB0, 0x92FCDDDB, 0x0F664BBAB,
    0x0E4F22022, 0x668742BF, 0x0C781B9C0, 0x413FFF55, 0x647974ED,
    0x9461ABEA, 0x0D49BFC02, 0x48432FE, 0x67EC10CE, 0x0A952CB34,
    0x231110C9, 0x840B87C9, 0x0FDC9CD5E, 0x0F42E0EDE, 0x5E9010D,
    0x6950DEDC, 0x0C10E0108, 0x2475DED7, 0x545E15D4, 0x949865EC,
    0x0F592DCED, 0x6F512281, 0x0B0B7DCE8, 0x0C272CF16, 0x25DAACE6,
    0x3295562B, 0x95FD33FB, 0x0C5E56AF7, 0x512233F6, 0x0B21CAAF6,
    0x0C3D79D25, 0x456CBFC2, 0x33FA243A, 0x97620209, 0x0EF1F2435,
    0x52870204, 0x826F3900, 0x2D3FBE99, 0x23A4001A, 0x5D33DD77,
    0x0DEC90014, 0x0F083F243, 0x721914E0, 0x83D4070F, 0x0C40E5727,
    0x0F3F68E24, 0x5E98AB86, 0x0E02DCE23, 0x0F1E8C052, 0x737DE2EF,
    0x0AD0DC04C, 0x0E3A06A04, 0x1D304761, 0x96EE8CF6, 0x0B0805C2D,
    0x1B22798F, 0x0B30E03F1, 0x8B4500A4, 0x0CDA2341, 0x1E951570,
    0x0A02A380D, 0x0B1E52A3C, 0x2BA36FD0, 0x2207B150, 0x8CA9CEB3,
    0x24955915, 0x1FF9E37E, 0x0B7E56DE0, 0x0DB1EE379, 0x5CB40616,
    0x4B416A8E, 0x5CFC5CBD, 0x25FA2723, 0x37B51952, 0x0B94A3BEF,
    0x0DC83B188, 0x5141AE33, 0x4CA6389D, 0x0CE3B5B3A, 0x0DFF64D68,
    0x19862AC6, 0x0E283F52D, 0x0FC15C464, 0x52A67C41, 0x4E0B06AB,
    0x0E5F6910D, 0x93006A6, 0x0A11B9108, 0x9C801B72, 0x8B0D7FEA,
    0x540B4A50, 0x6D9D1987, 0x0E75B5F1C, 0x20EB3C7A, 0x0A2805F17,
    0x7AB75BC9, 0x0AA9F92C6, 0x0E077095, 0x65C492C0, 0x10951859,
    0x2A26E791, 0x0A3E52D25, 0x9272919D, 0x1407B43A, 0x25C2A669,
    0x672960CF, 0x0CA913E9F, 0x41E21D64, 0x0A549FB34, 0x9BAE3CB4,
    0x156C8249, 0x56D33CAF, 0x0D0918243, 0x0E24C7472, 0x0D8B0B5F2,
    0x3C1893C2, 0x0DB3E5DB7, 0x3EA63B86, 0x6E8E7282, 0x0D1F65052,
    0x0EB881F89, 0x0DA158401, 0x53D3C995, 0x953A83FC, 0x0EF8C990,
    0x6FF34091, 0x0D35B1E60, 0x0E015C7A6, 0x437DA575, 0x84E45FDC,
    0x969F520A, 0x105D979F, 0x29EF66D6, 0x80801EB4, 0x94F80E3,
    0x6CB75EB3, 0x9C9F95AF, 0x98042019, 0x2FEFAA7B, 0x53292014,
    0x89BBC9CB, 0x0C34BA729, 0x8C49718F, 0x9E0463BE, 0x16C418D,
    0x592963B9, 0x3160606B, 0x0C94BEACD, 0x0C4B07537, 0x5C9BFF99,
    0x58008A03, 0x20FE5469, 0x5A8E31C7, 0x0B8F5C6AD, 0x0CAB0B8DC,
    0x0DC6BAB0B, 0x5E00CDA8, 0x813A4340, 0x1925CDA3, 0x0F15CCA55,
    0x0BA5A94BC, 0x0CC1586EA, 0x0DDD07919, 0x873A86E5, 0x98F57914,
    0x0F75D0DFA, 0x0F2C19864, 0x8AAD22C6, 0x0CD7A54F9, 0x4F0F7796,
    0x889F54F4, 0x9A5A4722, 0x0F8C1DC09, 0x0A7CCE37, 0x8C11F0D4,
    0x0C5A1CE32, 0x4736F0CF, 0x8A042302, 0x21EFAD64, 0x0FA26AA17,
    0x13B8794E, 0x8D76BEE3, 0x0C7069C41, 0x489BBEDE, 0x37292356,
    0x0B8BE45F3, 0x0FB8B7826, 0x53489A51, 0x0B6B07821, 0x0E698AF1D,
    0x4A008CEC, 0x388DF164, 0x0BA231401, 0x0F3B2F15F, 0x754813FC,
    0x0B815462F, 0x0E7FD7D2B, 0x4B655AFB, 0x58200440, 0x0BB87E210,
    0x0F517BF6D, 0x76ACE20B, 0x8867D439,
};
