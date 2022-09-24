#!/usr/bin/php
<?php

require_once('/opt/kwynn/kwutils.php');

new sntpWrapper();

class sntpWrapper {
	
const bin	    = __DIR__ . '/' . 'sntp';
const maxS = 1;
const NISTMaxS = 4;

function __construct() {
    $ns = $this->get();
    $this->sanityCheck($ns);
    $this->sleep($ns[0]);
}

private function sleep($b) {
    
    $ss = self::NISTMaxS - ((nanotime() - $b) / M_BILLION);
    if ($ss < 0) return;
    $sus = roint($ss * M_MILLION);
    usleep($sus); 
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


private function get() {
    $r = shell_exec(self::bin);
    echo($r);
    $a = explode("\n", $r);
    $a20 = array_slice($a, 0, 4);
    $a30 = [];
    foreach($a20 as $v) $a30[] = intval($v);
    return $a30;
    
    

}




} // class
