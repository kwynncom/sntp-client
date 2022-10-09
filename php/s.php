#! /usr/bin/php
<?php

require_once('/opt/kwynn/kwutils.php');

class sntp_wrapper {
	
	const cmdBase = 'sntp -nosleep'; // sleeping in PHP.  If I sleep in C, then the PHP sanity check fails, and I don't get my output for 4 seconds.
	const maxNISTS = 4;
	const tchars   = 79;
	const lockf    = '/var/kwynn/mysd/lock';
	const fifoo    = '/var/kwynn/mysd/poke';
	const fifoi    = '/var/kwynn/mysd/get';
	const versions = '10/09 00:15 rc2 - common sanity check';
	
	private function checkStart() {
		$isd = $this->isd;
		
		if ($isd) {
			$plock = new sem_lock(__FILE__);
			$plock->lock();
		}
		
		kwas(is_readable(self::lockf), 'lock file does not exist or is not readable - sntpd kw');
		$h = fopen(self::lockf, 'r');
		$lok = flock($h, LOCK_EX | LOCK_NB);
		flock($h, LOCK_UN);
		fclose($h);
		if ($isd && $lok) kwnohup('sntp -d -fifoout -nosleep');
		if ($isd) $plock->unlock();
		if (!$lok) $this->isd = true;
		
	}
	
	private function rund() {
		$this->checkStart();
		$doc = isset($this->doclose);
		if   ($doc) { $tos = 'x'; echo("Shutting down...\n"); }
		else		$tos = 'a';
		
		if ($this->isd || $doc) file_put_contents(self::fifoo, $tos);
		if ($doc) exit(0);
		
		if ($this->isd) $this->doout(file_get_contents(self::fifoi), 'fifo');
	}
	
	
	
	private function procArgs() {
		global $argv, $argc;

		$this->dojson = false;
		$this->isd  = false;
		
		for($i = 1; $i < $argc; $i++) {
			switch($argv[$i]) {
				case '-json' : $this->dojson = true; break;
				case '-d'    : $this->isd  = true; break;
				case  'x'	 :
				case '-x'    : $this->doclose = true;
					
			}
		}
	}
	
	public function __construct() {
	
		try {
			$this->procArgs();
			$js = $this->dojson;
			$this->setCmd();
			$this->rund();
			$this->runShell(); // must check for lock first, whether -d or not
			$this->maybeSleep();
		} catch(Exception $ex) {
			if (!$this->dojson) throw $ex;
		};
		
		$this->jsonf();

	}

	
	private function runShell() {
		if ($this->isd) return false;
		$t = shell_exec($this->cmdo);
		$this->doout($t, 'single shell_exec');
	}
	
	private function doout(string $t, string $from) {
				
		$a = $this->popValid($t);

		if (!$this->dojson) { 
			echo($t);
			$min = min($a);
			foreach($a as $n) echo(number_format($n - $min) . "\n");
			echo(number_format(self::SNTPOffset($a)) . " = offset\n");
			echo(number_format($a[1] - $a[0]) . " = out\n");
			echo(number_format($a[3] - $a[2]) . " = in\n" );
			echo('Source: ' . $from . "\n");
			$this->outVersion();
			echo('Results **OK**' . "\n");
		}		

		$this->jsonf();
	}
		
	private function outVersion() {
		$s  = '';
		$s .= 'PHP file date: '  . date('r', filemtime(__FILE__)) . "\n";
		$s .= 'PHP version msg: ' . self::versions . "\n";
		echo($s);
	}
	
	private function jsonf() {
		
		if (!$this->dojson || isset($this->jsonout)) return;
		
		$ret['status'] = false;
		if (isset($this->ot4)) {
			$ret['status'] = 'OK';
			$ret['t4'] = $this->ot4;
		}
		
		$ret['ip'] = $this->oip;
		
		echo(json_encode($ret, JSON_PRETTY_PRINT));
		$this->jsonout = true;
	}
	
	private function maybeSleep() {
		if (!$this->dosleep) return;
		if (!$this->dojson) echo('PHP sleeping.  Zzzz....' . "\n");
		sleep(self::maxNISTS);		
	}
		
	public static function SNTPOffset($T) {
		$t = ((($T[1] - $T[0]) + ($T[2] - $T[3]))) >> 1;
		return $t;
	}
	
	private function setCmd() {
		global $argc, $argv;
				
		$c  = self::cmdBase . ' ';
		if ($argc > 1) $c .= implode(' ', array_slice($argv, 1));	
		$this->cmdo = $c;
		
		$this->dosleep = true;		
		if ($argc <= 1) return;
		for ($i=1; $i < $argc; $i++) if ($argv[$i] === '-nosleep') { $this->dosleep = false; break; }
	}
	
	private function popValidTs($a) {
	/*	$n = self::tlines;
		kwas(count($a) === $n, 'bad tline count sntp sanity 2');
		for ($i=0; $i < $n; $i++) $a[$i] = intval($a[$i]);
		kwas(count($a) >= 4, 'fail - for immediate tline sntp sanity purposes');
		
		$min = min($a);
		$max = max($a);
		kwas($max - $min < M_BILLION, 'time sanity check fails');
		$ds = abs(nanotime() - $max);
		kwas($ds < M_BILLION , 'time sanity check fail 2 - perhaps quota fail');
		kwas($a[1] <= $a[2], 'server time sanity check fail between in and out');
		kwas($a[0] <  $a[3], 'server time sanity check internal out and in');
		$this->ot4 = $a;
		return $a; */
	}
	
	private function popValid($t) {
		$ret = SNTPTextToArr ($t);
		kwas($ret, 'SNTP text to array fail');
		$a = $this->ot4 = $ret['Uns4'];
		$this->oip = $ret['ip'];
		return $a;
	}
	
}

new sntp_wrapper();

function SNTPTextToArr($t) {
	
	static $tln = 4;
	static $ipi = 4;
	static $tol = M_BILLION;
	
	try {
		$a  = explode("\n", trim($t)); unset($t); kwas($a && is_array($a) && count($a) >= $tln, 'wrong lines sntp sanity check'); 
		$ip = getValidIPOrFalsey(kwifs($a, $ipi)); 
		$a = array_slice($a, 0, $tln);

		$n = $tln;
		kwas(count($a) === $n, 'bad tline count sntp sanity 2');
		for ($i=0; $i < $n; $i++) $a[$i] = intval($a[$i]);
		kwas(count($a) >= 4, 'fail - for immediate tline sntp sanity purposes');
		
		$min = min($a);
		$max = max($a);
		kwas($max - $min < $tol, 'time sanity check fails');
		$ds = abs(nanotime() - $max);
		kwas($ds < $tol , 'time sanity check fail 2 - perhaps quota fail');
		kwas($a[1] <= $a[2], 'server time sanity check fail between in and out');
		kwas($a[0] <  $a[3], 'server time sanity check internal out and in');
		
		$ret['ip'  ] = $ip;
		$ret['Uns4'] = $a;

		return $ret;
	} catch (Exception $ex) {
		return false;
	}
}
