
#include <srp-utils.h>

int ngtype_from_string(const char *str, SRP_NGType *ngtype) {
    if(strcmp(str, "ng1024") == 0) {
	    *ngtype = SRP_NG_1024;
	    return 0;
	}
    if(strcmp(str, "ng2048") == 0) {
	    *ngtype = SRP_NG_2048;
	    return 0;
	}
    return 1;
}

int halgo_from_string(const char *str, SRP_HashAlgorithm *algo) {
    if(strcmp(str, "sha1") == 0) {
	    *algo = SRP_SHA1;
		return 0;
	}
    if(strcmp(str, "sha224") == 0) {
	    *algo = SRP_SHA224;
		return 0;
	}
    if(strcmp(str, "sha256") == 0) {
	    *algo = SRP_SHA256;
		return 0;
	}
    if(strcmp(str, "sha384") == 0) {
	    *algo = SRP_SHA384;
		return 0;
	}
    if(strcmp(str, "sha512") == 0) {
	    *algo = SRP_SHA512;
		return 0;
	}
	return 1;
}

void bail(const char *message) {
	puts(message);
	exit(1);
}

uint8_t hex2nibble(char c) {
    if(c >= '0' && c <= '9') {
        return c - '0';
    } else if (c >= 'a' && c <= 'f') {
        return c - 'a' + 10;
    } else if (c >= 'A' && c <= 'F') {
        return c - 'A' + 10;
    } else {
        bail("Invalid hex");
        return 0;
    }
}

static const char hextable[16] = {
    '0', '1', '2', '3', '4', '5', '6', '7',
    '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'
};
char nibble2hex(uint8_t n) {
    return hextable[n & 0xf];
}

void hex2bin(const char *src, size_t srclen, uint8_t *dst, size_t *dstlen) {
    if(srclen % 2) {
        bail("Invalid hex length");
    }
    size_t newlen = 0;
    off_t si, di;
    for(si = 0, di = 0; si < srclen && di < *dstlen; si += 2, di += 1) {
        uint8_t a = hex2nibble(src[si + 0]);
        uint8_t b = hex2nibble(src[si + 1]);
        dst[di] = (a << 4) | (b);
        newlen++;
    }

    if(di == *dstlen && si < srclen) {
        bail("Insufficient space for input string");
    }

    *dstlen = newlen;
}

void bin2hex(const uint8_t *src, size_t srclen, char *dst, size_t *dstlen) {
    if(*dstlen < (srclen * 2 + 1)) {
        bail("Insufficient space for output string");
    }
    size_t newlen = 0;
    off_t si, di;
    for(si = 0, di = 0; si < srclen && di < *dstlen; si += 1, di += 2) {
        char a = nibble2hex(src[si] >> 4);
        char b = nibble2hex(src[si] & 0xf);
        dst[di + 0] = a;
        dst[di + 1] = b;
        newlen += 2;
    }

    dst[di] = 0;
    
    *dstlen = newlen;
}

size_t hexask(const char *label, uint8_t *dst, size_t len) {
    printf("Enter %s: ", label);
    size_t l = len * 2;
    char *str = malloc(l);
    if(getline(&str, &l, stdin) < 0) {
        bail("Could not read value");
    }
    size_t f = len;
    hex2bin(str, strlen(str) - 1, dst, &f);
    return f;
}

size_t hexread(char *src, uint8_t *dst, size_t len) {
    size_t f = len;
    hex2bin(src, strlen(src), dst, &f);
    return f;
}

void hexdump(const char *label, const uint8_t *src, size_t size) {
    size_t hexsize = size * 2 + 1;
    char *hexstr = alloca(hexsize);
    bin2hex(src, size, hexstr, &hexsize);
    printf("%s: %s\n", label, hexstr);
}
