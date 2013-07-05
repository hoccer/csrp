
#include "srp-utils.h"

int
main(int argc, char **argv)
{
    /* enough arguments? */
    if(argc != 5) {
        printf("Usage: %s <halgo> <ngtype> <user> <pass>\n", argv[0]);
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
    char *pass = argv[4];

    /* create SRP user instance */
    struct SRPUser *u = srp_user_new
        (halgo, ngtype, user,
         (const unsigned char *)pass, strlen(pass),
         NULL, NULL);

    /* we will need the SKL */
    int skl = srp_user_get_session_key_length(u);

    /* generate A */
    const char *I = NULL;
    const unsigned char *A = NULL;
    int Alen = 0;
    srp_user_start_authentication(u, &I, &A, &Alen);
    hexdump("A", A, Alen);

    /* ask for B from server */
    uint8_t *B = alloca(1024);
    size_t Blen = hexask("B", B, 1024);
    hexdump("B", B, Blen);

    /* ask for salt from server or local state */
    uint8_t *s = alloca(1024);
    size_t slen = hexask("s", s, 1024);
    hexdump("s", s, slen);

    /* process B generating M1 */
    const unsigned char *M1 = NULL;
    int M1len = 0;
    srp_user_process_challenge(u, s, slen, B, Blen, &M1, &M1len);
    hexdump("M1", M1, M1len);

    /* ask for M2 from server */
    uint8_t *M2 = alloca(1024);
    size_t M2len = hexask("M2", M2, 1024);
    hexdump("M2", M2, M2len);

    /* verify the session key */
    srp_user_verify_session(u, M2);

    /* check if we have been authenticated */
    int isauth = srp_user_is_authenticated(u);
    if(isauth) {
        puts("Authenticated");
        /* show the session key */
        const unsigned char *S = srp_user_get_session_key(u, NULL);
        hexdump("S", S, skl);
    } else {
        bail("Authentication failed");
    }

    /* clean up */
    srp_user_delete(u);
    
    return 0;
}
