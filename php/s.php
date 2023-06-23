<?php

require_once('/opt/kwynn/kwutils.php');

// 2 classes:
// 1. The sntp_wrapper that calls the C version
// 1.5 - Not a class, but the call to the wrapper
// 2. That version of the "sanity" class to check the result

class sntp_wrapper {
	
	const cmdBB = 'sntpkw';
	const cmdBase   = self::cmdBB  . ' -nosleep'; // sleeping in PHP.  If I sleep in C, then the PHP sanity check fails, and I don't get my output for 4 seconds.
	const cmdNohup  =  self::cmdBB . ' -d -fifoout -nosleep';
	const cmdrunner = 'sntpr';
	const maxNISTS = 4;
	const lockf    = '/var/kwynn/mysd/lockC'; // must match C
	const flf	   = '/var/kwynn/mysd/fullLog.txt'; // must match C
	const tailn    = 25;
	const starts   = '*** BEGIN ***';
	const versions = '2023/01/20 03:20 - pull from log; working with shell_exec'; 
	
	public function __destruct() {
		if (kwifs($this, 'plock')) $this->plock->unlock();	
	}
	
	private function checkStart() {
		$isd = $this->isd;
		
		if ($isd) {
			$this->plock = new sem_lock(__FILE__);
			$this->plock->lock();
		}

		$lok = $this->iGetLock();

		if ($isd && $lok) {
			kwnohup(self::cmdNohup);
			// usleep(200000);
		}
		if ($isd) $this->plock->unlock();
		if (!$lok) $this->isd = true;
		
	}
	
	private function iGetLock() {
		kwas(is_readable(self::lockf), 'lock file does not exist or is not readable - sntpd kw');
		$h = fopen(self::lockf, 'r');
		$lok = flock($h, LOCK_EX | LOCK_NB);
		flock($h, LOCK_UN);
		fclose($h);
		return $lok;
	}
	
	private function rund() {
		$this->checkStart();
		
		if (!$this->isd) return;
		
		$doc = isset($this->doclose);
		if   ($doc) $tos = 'x'; 
		else		$tos = 'a';
		
		$rres = shell_exec(self::cmdrunner . ' ' . $tos);
		if (kwifs($this, 'plock')) $this->plock->unlock();

		if ($doc) {
			echo("Shutting down (from PHP)...\n");
			exit(0);
		}
		
		$this->doout($rres, 'fifo');
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
		$t = $this->safeCall();
		$this->doout($t, 'single shell_exec');
	}
	
	private function safeCall() {
		$t = shell_exec($this->cmdo);
		if (!$t) $t = '';
		return $t;
	}
	
	private function getFromFullLog() {
		$t = shell_exec('tail -n ' . self::tailn . ' ' . self::flf);
		$a = explode("\n", $t);
		for ($i = count($a) - 1; $i >= 0; $i--) {
			if (strpos($a[$i], self::starts) !== false) { 
				return implode("\n", array_slice($a, $i + 1));
			}
		}
		return '';
	}
	
	private function doout(string $ignore, string $from) {
		
		$t = $this->getFromFullLog();
		
		if (!$this->dojson) echo($t);
				
		$a = $this->popValid($t);

		if (!$this->dojson) { 
			$min = min($a);
			foreach($a as $n) echo(number_format($n - $min) . "\n");
			echo(number_format(self::SNTPOffset($a)) . " = offset\n");
			echo(number_format($a[1] - $a[0]) . " = out\n");
			echo(number_format($a[3] - $a[2]) . " = in\n" );
			echo('Source: ' . $from . "\n");
			$this->outVersion();
			if ($this->sane)
				 echo('Results **OK**' . "\n");
			else echo("sanity failures\n");
			echo('Sanity VERSION: ' . sntpSanityUns4::ssVersion . "\n");
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
	
	private function popValid($t) {
		$this->sane = false;
		$a = [];
		try {
			$ret = sntpSanityUns4::ck($t, !$this->dojson);
			if (!$ret) {
				if (!$this->dojson) echo("**Fail\n");
				exit(153);
			}
			$a = $this->ot4 = $ret['Uns4'];
			$this->oip = $ret['ip'];
			$this->sane = $ret['sane'];
		} catch(Exception $ex) {
			if ($this->dojson) throw $ex;
			echo("PHP Santiy check failed with " . $ex->getMessage() . "\nRaw Text: $t\n");
			exit(117);
		}
		return $a;
	}
	
}

new sntp_wrapper();

class sntpSanityUns4 {
	
	const tln   = 4;
	const ipi   = self::tln;
	const tolns = M_BILLION;
	const ssVersion = '2023/01/20 19:26 - printing PHP hutime';
	
	public static function ck(string $t, bool $contiff = false) {
		$o = new self($t, $contiff);
		return $o->getRes();
		
	}
	
	private function __construct(string $t, bool $contiff) {
		$this->contif  = $contiff;
		$this->oret = false;
		$this->sanFail = false;
		$this->oret = $this->ttoa($t);
	}
	
	public function getRes() {
		return $this->oret;
	}
	
	private function ass(bool $tock, string $msg, bool $failAnyhow = false) {
		if ($tock) return true;
		$this->sanFail = true;
		if ($this->contif && !$failAnyhow) echo("**FAIL: " . $msg . "\n");
		else throw new Exception($msg);
		
	}
	
	private function ttoa(string $t) {
		
		static $failv = [];

		try {
			
			$o = $this;
			
			if (!$o->ass($t, 'blank string')) return $failv;
			
			$a  = explode("\n", trim($t));
			$o->ass($a && is_array($a) && count($a) >= self::tln, 'wrong lines sntp ck - out: ' . "$t");  unset($t); 
			$ip = getValidIPOrFalsey(kwifs($a, self::ipi)); 
			$a = array_slice($a, 0, self::tln);

			$n = self::tln;
			$o->ass(count($a) === $n, 'bad tline count sntp sanity 2', true);
			for ($i=0; $i < $n; $i++) {
				if (is_numeric($a[$i])) 
					 $a[$i] = intval($a[$i]);
				else kwas(false, 'non-numeric value sent to time array 4 Uns');
			}

			$o->ass(count($a) >= 4, 'fail - for immediate tline sntp sanity purposes', true);

			$min = min($a);
			$max = max($a);
			$o->ass($max - $min < self::tolns, 'time sanity check fails');
			$nowns = nanotime();
			$ds = abs($nowns - $max);
			$o->ass($ds < self::tolns, 'time sanity check fail 2 - perhaps quota fail; now ns PHP = ' . date('H:i:s', roint($nowns / M_BILLION)));
			$o->ass($a[1] <= $a[2], 'server time sanity check fail between in and out');
			$o->ass($a[0] <  $a[3], 'server time sanity check internal out and in');

			$ret['ip'  ] = $ip;
			$ret['Uns4'] = $a;
			$ret['sane'] = !$this->sanFail;

			return $ret;
		} catch (Exception $ex) {	}
		
		return $failv;
	} // func
	
	public static function SNTPOffset($T) {	return ((($T[1] - $T[0]) + ($T[2] - $T[3]))) >> 1;	}

} // class
