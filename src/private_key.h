#ifndef PRIVATE_KEY_H_
#define PRIVATE_KEY_H_
unsigned char example_key_DER[] = {
  0x30, 0x82, 0x02, 0x5f, 0x02, 0x01, 0x00, 0x02, 0x81, 0x81, 0x00, 0xea,
  0xae, 0x54, 0x39, 0x02, 0x0f, 0x7c, 0x5f, 0x4b, 0xea, 0xd3, 0x50, 0x35,
  0xf3, 0x95, 0x81, 0x41, 0x12, 0x11, 0xb9, 0x6f, 0x6d, 0x14, 0xa2, 0x77,
  0x35, 0xf7, 0x7c, 0x2f, 0xc4, 0x3e, 0x1e, 0xd6, 0xb0, 0x61, 0xc2, 0xb2,
  0x9e, 0x75, 0x26, 0x41, 0xcb, 0x9f, 0xa2, 0x1a, 0xea, 0xef, 0x04, 0xd1,
  0x1b, 0xf0, 0xb6, 0x94, 0x4a, 0xdb, 0xcc, 0xe3, 0x85, 0x8b, 0x52, 0xc6,
  0xb9, 0x89, 0xe6, 0x56, 0x73, 0x30, 0xad, 0x6d, 0xd7, 0x35, 0x50, 0x83,
  0x5b, 0x73, 0xd6, 0xfd, 0x58, 0x6c, 0xda, 0x97, 0xb2, 0x1d, 0x8a, 0xe7,
  0x57, 0xe3, 0xb5, 0x72, 0x8d, 0x44, 0x44, 0x12, 0x62, 0xc2, 0x6c, 0xc1,
  0x9e, 0x4a, 0x44, 0x34, 0xdd, 0xef, 0x36, 0x01, 0xd2, 0x4b, 0xb2, 0x7e,
  0x94, 0x22, 0x5e, 0xb9, 0xfc, 0xf5, 0x07, 0x84, 0x83, 0x86, 0x5a, 0xed,
  0x95, 0x01, 0xed, 0x25, 0x38, 0xd8, 0x25, 0x02, 0x03, 0x01, 0x00, 0x01,
  0x02, 0x81, 0x81, 0x00, 0x8e, 0x33, 0x69, 0xe7, 0x4d, 0x3f, 0x52, 0xee,
  0x6f, 0x92, 0xef, 0xce, 0x37, 0x92, 0xcc, 0x71, 0x90, 0x9b, 0x41, 0x50,
  0x2a, 0x9b, 0x5f, 0x18, 0x7e, 0xc1, 0x9d, 0x9f, 0x8b, 0x56, 0x0f, 0x0a,
  0x22, 0xae, 0xa7, 0xde, 0x10, 0xdb, 0x81, 0x42, 0xd8, 0x35, 0x69, 0x1a,
  0x11, 0xd3, 0x6f, 0x57, 0x6c, 0xd9, 0xd0, 0x46, 0xe1, 0x9e, 0x66, 0x87,
  0x45, 0x0a, 0x23, 0x7c, 0xd9, 0xdb, 0x8d, 0x19, 0x73, 0x17, 0x57, 0xf9,
  0x64, 0xe6, 0xee, 0x4a, 0x7a, 0x2b, 0xdd, 0x0c, 0xd1, 0xd3, 0xc0, 0x6f,
  0x7a, 0x5b, 0xd1, 0xa8, 0xee, 0x58, 0xf0, 0xf5, 0xb6, 0x34, 0xf7, 0xdc,
  0x64, 0x35, 0x3c, 0xc4, 0xc8, 0xb7, 0xa3, 0xbb, 0xa2, 0xb6, 0x83, 0x4d,
  0xbd, 0x0c, 0x3f, 0xd8, 0xb0, 0x38, 0xd7, 0xda, 0xcb, 0xe0, 0xdd, 0x5f,
  0x7b, 0xcd, 0x6f, 0x86, 0xde, 0xdb, 0x76, 0x31, 0x05, 0xcc, 0x18, 0x81,
  0x02, 0x41, 0x00, 0xf5, 0xd3, 0xce, 0x4e, 0xeb, 0xae, 0x12, 0xab, 0xe3,
  0x27, 0xd9, 0x7a, 0xe5, 0xd3, 0x8a, 0x14, 0x91, 0xb2, 0x65, 0xc5, 0x5b,
  0xc2, 0xbb, 0x7b, 0x61, 0x15, 0xec, 0x6c, 0x79, 0xa4, 0x79, 0x07, 0x73,
  0x2f, 0x36, 0x90, 0x94, 0x3e, 0x38, 0x32, 0xce, 0x63, 0xa5, 0xc6, 0xd4,
  0x88, 0x55, 0xd6, 0x8f, 0x4b, 0xa8, 0x87, 0x37, 0xfd, 0x9e, 0x89, 0x6b,
  0x0e, 0xbc, 0x67, 0x0d, 0xae, 0xfb, 0xe1, 0x02, 0x41, 0x00, 0xf4, 0x64,
  0x71, 0x5a, 0x4b, 0x0d, 0xa2, 0x0b, 0x4c, 0x24, 0x78, 0xaf, 0x53, 0x0c,
  0xae, 0x68, 0x10, 0xdf, 0x52, 0xdd, 0x71, 0x51, 0xf3, 0x18, 0xac, 0x83,
  0xcd, 0xce, 0x46, 0x43, 0x02, 0x4b, 0xda, 0x27, 0x7d, 0xc3, 0xf2, 0x81,
  0xe1, 0x49, 0x9b, 0xaf, 0xb3, 0x82, 0xa6, 0x29, 0x6d, 0x4a, 0xe1, 0x81,
  0xbb, 0xff, 0xb8, 0x87, 0x8f, 0x68, 0x77, 0x84, 0x9e, 0xf5, 0xe1, 0xd1,
  0x84, 0xc5, 0x02, 0x41, 0x00, 0xd6, 0x21, 0xb4, 0xc1, 0x3c, 0x3c, 0x7a,
  0x59, 0x2d, 0xda, 0xa5, 0x6f, 0x8f, 0x48, 0xd0, 0x2b, 0x90, 0xaa, 0x44,
  0x4a, 0x40, 0xdf, 0x04, 0xae, 0xb9, 0x67, 0xfe, 0x74, 0x93, 0x15, 0x38,
  0xb3, 0xcb, 0x9a, 0xe1, 0xbd, 0xd8, 0xd3, 0xc5, 0xe2, 0x43, 0xb3, 0xbb,
  0x50, 0xd2, 0x0b, 0x1c, 0x32, 0x74, 0xc9, 0xe3, 0xb8, 0x3d, 0x86, 0x77,
  0x95, 0x9e, 0x22, 0xcc, 0xec, 0x3b, 0x98, 0x24, 0x21, 0x02, 0x41, 0x00,
  0x80, 0xa2, 0x69, 0xa3, 0x6c, 0x03, 0xb5, 0xfa, 0x1a, 0x4e, 0xcc, 0x13,
  0x04, 0x34, 0x95, 0x8c, 0x02, 0xed, 0x79, 0x11, 0x43, 0x77, 0x2d, 0xec,
  0x1b, 0xc9, 0x98, 0x46, 0xed, 0x66, 0x91, 0xcb, 0xd8, 0xfc, 0x96, 0x4e,
  0xcf, 0xd3, 0x1b, 0x00, 0x82, 0x68, 0x9d, 0xa4, 0xb3, 0xce, 0xf4, 0x4e,
  0xc8, 0x99, 0x5f, 0x99, 0xbe, 0xc7, 0xe3, 0x6c, 0xca, 0x13, 0xcd, 0x13,
  0x80, 0x93, 0xa6, 0x3d, 0x02, 0x41, 0x00, 0xf2, 0x24, 0xb5, 0xd8, 0x90,
  0x44, 0x46, 0xf2, 0x9e, 0xaf, 0xcf, 0xa4, 0x5a, 0x4e, 0xd8, 0x56, 0x39,
  0xc1, 0xe4, 0xd4, 0x41, 0x27, 0x41, 0xbd, 0x31, 0x72, 0x18, 0x40, 0xf3,
  0x02, 0x23, 0x65, 0xcf, 0x57, 0x67, 0x5a, 0x4e, 0x03, 0xe9, 0x50, 0x41,
  0x4d, 0xcd, 0x40, 0xc4, 0x63, 0xca, 0x06, 0x56, 0xce, 0xc3, 0x9a, 0x53,
  0x77, 0xa0, 0x52, 0x03, 0xd9, 0x41, 0x09, 0x89, 0xd2, 0xab, 0x0c
};
unsigned int example_key_DER_len = 611;
#endif
