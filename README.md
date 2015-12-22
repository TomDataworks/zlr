# ZLR - ZNC Log Ripper Forensic Utility
This is a hacked together program for ripping out ZNC logs from any
arbitrary bitstream. You could use the zlr utility to decrypt a ZNC
log, the zlra.sh utility simply wraps zlr with a progress meter and
wraps the output into a tar file.

The zlr utility takes a single argument, password. It reads data from
standard input and outputs decrypted data to standard output.

The zlra utility takes three arguments. The first is the password,
the second is an input file or block device, and the third is the
tar archive to dump logs to.
