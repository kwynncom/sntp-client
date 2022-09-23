#!/usr/bin/php
<?php

require_once('/opt/kwynn/kwutils.php');

new sntpWrapper();

class sntpWrapper {
	
const bin	    = __DIR__ . '/' . 'sntp';
const expectLen = 64;
const maxS = 1;
const NISTMaxS = 4;

function __construct() {
    $dat = $this->get();
    $ns = $this->parse($dat); unset($dat);
    $this->sanityCheck($ns);
    $this->out($ns); 
    $this->sleep($ns[0]);
}

private function sleep($b) {
    
    $ss = self::NISTMaxS - ((nanotime() - $b) / M_BILLION);
    if ($ss < 0) return;
    $sus = roint($ss * M_MILLION);
    usleep($sus); 
}

private function out($ns) {
    echo(json_encode($ns) . "\n");
}

private function sanityCheck($a) {
    $min = min($a);
    $max = max($a);
    kwas($max - $min < M_BILLION, 'time sanity check fails - ck 1 0417');
    $ds = abs(nanotime() - $max) / M_BILLION;
    kwas($ds < self::maxS , 'time sanity check fail 2');
    kwas($a[1] <= $a[2], 'server time sanity check fail between in and out');
    kwas($a[0] <  $a[3], 'server time sanity check internal out and in');
    $del = ($a[3] - $a[0]) / M_BILLION;
    return;
}

private static function getArgs() {
    global $argc;
    global $argv;

    if ($argc < 2) return '';
    return $argv[1];
}

private function get() {
    $a = self::getArgs();
    $r = popen(self::bin . ' ' . $a, 'rb'); unset($a);
    $wr = fread($r, self::expectLen); kwas(strlen($wr) === self::expectLen, 'sntp wrap fread not ' . self::expectLen . 'bytes');
    pclose($r); unset($r);
    return $wr;
}

private function parse($wr) {
    $ns[] = self::decodeSNTPP($wr, 48, 'Q');
    $ns[] = self::decodeSNTPP($wr, 32, 'N2');
    $ns[] = self::decodeSNTPP($wr, 40, 'N2');
    $ns[] = self::decodeSNTPP($wr, 56, 'Q');
    return $ns;
}

private static function decodeSNTPP($p, $off, $unf) {

    static $UminusNTP = 2208988800;
    static $full32    = 4294967295;

    $lp = substr($p, $off, 8);
    $upn = unpack($unf, $lp);

    $un1 = $upn[1];

    if ($unf === 'N2') {
            $su   = $un1 - $UminusNTP;
            $fr = $upn[2] / $full32;
            $ns = $su * M_BILLION + intval(round($fr * M_BILLION));
            $ret = $ns;
    } else if ($unf === 'Q') $ret = $upn[1];

    return $ret;
}
} // class
