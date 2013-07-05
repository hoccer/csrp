
#include "srp-utils.h"

int
main(int argc, char **argv)
{
    /* enough arguments? */
    if(argc != 6) {
        printf("Usage: %s <halgo> <ngtype> <user> <salt> <verifier>\n", argv[0]);
        return 1;
    }

    /* get arguments */
	SRP_NGType ngtype;
	SRP_HashAlgorithm halgo;
	if(halgo_from_string(argv[1], &halgo)) {
		bail("invalid halgo");
	}
	if(ngtype_from_string(argv[2], &ngtype)) {
		bail("invalid ng");
	}
    char *user = argv[3];
    char *salt = argv[4];
    char *veri = argv[5];

    /* convert s and v to binary */
    unsigned char *s = alloca(1024);
    size_t slen = hexread(salt, s, 1024);
    hexdump("s", s, slen);
    unsigned char *v = alloca(1024);
    size_t vlen = hexread(veri, v, 1024);
    hexdump("v", v, vlen);

    /* ask for A from client */
    uint8_t *A = alloca(1024);
    size_t Alen = hexask("A", A, 1024);
    hexdump("A", A, Alen);

    /* begin verification */
    const unsigned char *B = NULL;
    int Blen = 0;
    struct SRPVerifier *verifier =
        srp_verifier_new(halgo, ngtype, user,
                         s, slen, v, vlen,
                         A, Alen, &B, &Blen,
                         NULL, NULL);
    hexdump("B", B, Blen);

    /* we will need the SKL */
    size_t skl = srp_verifier_get_session_key_length(verifier);

    /* ask for M1 from client */
    uint8_t *M1 = alloca(skl);
    hexask("M1", M1, skl);
    hexdump("M1", M1, skl);
    
    /* verify M1 */
    const unsigned char *M2 = NULL;
    srp_verifier_verify_session(verifier, M1, &M2);
    if(M2 != NULL) {
        hexdump("M2", M2, skl);
    }

    /* check if the user has been authenticated */
    int isauth = srp_verifier_is_authenticated(verifier);
    if(isauth) {
        puts("Authenticated");
        const unsigned char *S = srp_verifier_get_session_key(verifier, NULL);
        hexdump("S", S, skl);
    } else {
        bail("Authentication failed");
    }

    /* clean up */
    srp_verifier_delete(verifier);

    return 0;
}
