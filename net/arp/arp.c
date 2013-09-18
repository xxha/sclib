#include <stdio.h>
#include <string.h> 		//memset()
#include <netinet/in.h>   	//struct sockaddr, sockaddr_in
#include <sys/socket.h> 	//AF_INET, socket()
#include <linux/if_arp.h>    	//struct arpreq
#include <errno.h>		//EINVAL
#include <linux/sockios.h>	//SIOCSARP


#define DFLT_AF "inet"

#define DEBUG 0

#define ARP_OPT_A (0x1)
#define ARP_OPT_p (0x2)
#define ARP_OPT_H (0x4)
#define ARP_OPT_t (0x8)
#define ARP_OPT_i (0x10)
#define ARP_OPT_a (0x20)
#define ARP_OPT_d (0x40)
#define ARP_OPT_n (0x80)
#define ARP_OPT_D (0x100)
#define ARP_OPT_s (0x200)
#define ARP_OPT_v (0x400 * DEBUG)

static const struct aftype *ap;    /* current address family */
static int sockfd;                 /* active socket descriptor */
static char *device;

struct aftype {
	const char *name;
	const char *title;
	int af;
	int alen;
	char *(*print) (unsigned char *);
	const char *(*sprint) (struct sockaddr *, int numeric);
	int (*input) (/*int type, */ const char *bufp, struct sockaddr *);
	void (*herror) (char *text);
	int (*rprint) (int options);
	int (*rinput) (int typ, int ext, char **argv);

	/* may modify src */
	int (*getmask) (char *src, struct sockaddr *mask, char *name);
};

static const struct aftype inet_aftype = {
	.name 	= "inet",
	.title 	= "DARPA Internet",
	.af 	= AF_INET,
	.alen 	= 4,
//	.sprint = INET_sprint,
//	.input 	= INET_input,
};

static const struct aftype *const aftypes[] = {
	&inet_aftype,
//	&unspec_aftype,
	NULL
};


/*check our protocol family table for this family*/
const struct aftype *get_aftype(const char *name)
{
	const struct aftype *const *afp;

	afp = aftypes;
	while (*afp != NULL) {
		if (!strcmp((*afp)->name, name))
			return (*afp);
		afp++;
	}
	return NULL;

}

static int arp_show(void)
{
	FILE *fp;
	int num, type, flags;
	char ip[128];
	char hwa[128];
	char mask[128];
	char line[128];
	char dev[128];


	fp = fopen("/proc/net/arp", "r");

	/* bypass first line, it's header */
	fgets(line, sizeof(line), fp);


	while(fgets(line, sizeof(line), fp)) {
		num = sscanf(line, "%s 0x%x 0x%x %s %s %s\n",
					ip, &type, &flags, hwa, mask, dev);
		printf("(%s) at %s on %s\n", ip, hwa, dev);
		
	}

	device = dev;
	return 0;

}

static unsigned hexchar2int(char c)
{
        if (isdigit(c))
                return c - '0';
        c &= ~0x20; /* a -> A */
        if ((unsigned)(c - 'A') <= 5)
                return c - ('A' - 10);
        return ~0U;
}

/* Input an Ethernet address and convert to binary. */
static int in_ether(const char *bufp, struct sockaddr *sap)
{
        unsigned char *ptr;
        char c;
        int i, errno;
        unsigned val;

        sap->sa_family = AF_INET;
        ptr = (unsigned char*) sap->sa_data;

        i = 0;
        while ((*bufp != '\0') && (i < ETH_ALEN)) {
                val = hexchar2int(*bufp++) * 0x10;
                if (val > 0xff) {
                        errno = EINVAL;
                        return -1;
                }
                c = *bufp;
                if (c == ':' || c == 0)
                        val >>= 4;
                else {
                        val |= hexchar2int(c);
                        if (val > 0xff) {
                                errno = EINVAL;
                                return -1;
                        }
                }
                if (c != 0)
                        bufp++;
                *ptr++ = (unsigned char) val;
                i++;

                /* We might get a semicolon here - not required. */
                if (*bufp == ':') {
                        bufp++;
                }
        }
        return 0;
}

static int arp_set(char **args)
{
	char *host, *hwaddr;
	struct arpreq req;
	int flags;

	memset(&req, 0, sizeof(struct arpreq));

	host = args[2];
	printf("xxha:-------- host = %s\n", host);

	if (!inet_aton(host, &req.arp_pa)) {
		printf("inet_aton error\n");
		return -1;
	}

	hwaddr = args[3];
	printf("xxha:-------- hwaddr = %s\n", hwaddr);
	if (in_ether(hwaddr, &req.arp_ha)) {
		printf("in ether hw addr error\n");
		return -1;
	}

	flags = ATF_PERM | ATF_COM;
	req.arp_flags = flags;

	strncpy(req.arp_dev, device, sizeof(req.arp_dev));
		
	if (ioctl(sockfd, SIOCSARP, &req) < 0) {
		printf("ioctl set arp error\n");
		return -1;
	}
	return 0;
}

static int arp_del(char **args)
{
	char *host;
	struct arpreq req;
	int flags = 0;
	int err = -1;

	memset(&req, 0, sizeof(req));

	host = args[2];
	printf("xxha:-------- host = %s\n", host);
	if (!inet_aton(host, &req.arp_pa)) {
		printf("inet_aton error\n");
		return -1;
	}

	req.arp_flags = ATF_PERM;
	strncpy(req.arp_dev, device, sizeof(req.arp_dev));

	err = ioctl(sockfd, SIOCDARP, &req);
	if (err < 0) {
		printf("No ARP entry for %s\n", host);
		return -1;
	}

	return 0;
}

static void arp_usage()
{
	printf("arp usage:\n");
	printf("	arp -a\n");
	printf("	arp -s ethx ipaddr mac\n");
	printf("	arp -d ipaddr\n");

}

int main(int argc, char **argv)
{
	char *hw_type;
	char *protocol;
	int flags = 0;
	if (argc < 2) {
		arp_usage();
		return -1;
	}

	/* initialize variables */
	ap = get_aftype(DFLT_AF);
	if (!ap)
		printf("%s: %s not supported\n", DFLT_AF, "address family");

	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sockfd < 0)
		printf("socket (AF_INET) error\n");

	printf("xxha:----- argv[1] = %s\n", argv[1]);
	if (!strcmp(argv[1],  "-a")) {
		flags |= ARP_OPT_a;
		arp_show();
	} else if (!strcmp(argv[1], "-s")) {
		flags |= ARP_OPT_s;
		arp_set(argv);
	} else if (!strcmp(argv[1], "-d")) {
		flags |= ARP_OPT_d;
		arp_del(argv);
	} else 
		arp_usage();

	return 0;
}



