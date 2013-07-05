
#include "srp-utils.h"

int
main(int argc, char **argv)
{
	SRP_NGType ngtype;
	SRP_HashAlgorithm halgo;

    if(argc != 5) {
        printf("Usage: %s <halgo> <ngtype> <user> <pass>\n", argv[0]);
        return 1;
    }

	if(halgo_from_string(argv[1], &halgo)) {
		bail("invalid halgo");
	}
	if(ngtype_from_string(argv[2], &ngtype)) {
		bail("invalid ng");
	}

    char *user = argv[3];
    char *pass = argv[4];

    uint8_t *salt = NULL;
    int saltsize = 0;
    uint8_t *veri = NULL;
    int verisize = 0;

    srp_create_salted_verification_key
        (halgo, ngtype, user,
         (unsigned char*)pass, strlen(pass),
         (const unsigned char**)&salt, &saltsize,
         (const unsigned char**)&veri, &verisize,
         0, 0);

    hexdump("salt", salt, saltsize);
    hexdump("verifier", veri, verisize);

    free(salt);
    free(veri);

    return 0;
}

