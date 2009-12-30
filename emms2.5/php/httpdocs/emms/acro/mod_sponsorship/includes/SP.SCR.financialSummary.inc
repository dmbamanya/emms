<?php
$show_gohome = true;

$data1 = WEBPAGE::$dbh->getAll(sprintf("
	  select
            z.zone,
            count(lmd.loan_id) loans,
            sum(l.kp) kp,
            if(sum(lwo.amount),sum(lwo.amount),0) write_off,
            if(sum(lcd.balance_kp),sum(lcd.balance_kp),0) balance,
            sum(l.kp) - if(sum(lcd.balance_kp),sum(lcd.balance_kp),0) - if(sum(lwo.amount),sum(lwo.amount),0) repayment
       from
            (tblZones z, tblLoansMaster lm, tblLoansMasterDetails lmd, tblLoans l)
       left join
            tblLoansCurrentData lcd on lcd.loan_id = lmd.loan_id
       left join
            tblLoanWriteOff lwo on lwo.loan_id = lmd.loan_id
       where
            lm.check_status = 'R' and
            lm.sponsor_id = %s and
            lmd.master_id = lm.id and
            z.id = lm.zone_id and
            l.id = lmd.loan_id
       group by
            lm.zone_id", WEBPAGE::$userID));

$data2 = WEBPAGE::$dbh->getAssoc(sprintf("
		 select
            z.zone,
            if(sum(p.principal),sum(p.principal),0) principal,
            if(sum(p.interest)+sum(p.fees)+sum(insurances)+sum(penalties),sum(p.interest)+sum(p.fees)+sum(insurances)+sum(penalties),0) rendimiento
       from
            (tblZones z,
            tblLoansMaster lm,
            tblLoansMasterDetails lmd)
       left join
            tblPayments p on p.loan_id = lmd.loan_id
       where
            lm.check_status = 'R'
            and lm.sponsor_id = %s and
            lmd.master_id = lm.id and
            z.id = lm.zone_id
       group by
            lm.zone_id", WEBPAGE::$userID));

$mrow['total']['zone'] = $_LABELS['total'];

foreach($data1 as $key=>$val) {

  $mrow['total']['loans'] 	 	 	+= $val['loans'];
  $mrow['total']['kp'] 	 	      += $val['kp'];
  $mrow['total']['avg'] 	 	     = round($$mrowata['total']['kp']/$mrow['total']['loans'],2);
  $mrow['total']['repayment'] 	+= $val['repayment'];
  $mrow['total']['write_off']   += $val['write_off'];
  $mrow['total']['balance'] 	 	+= $val['balance'];
  $mrow['total']['rendimiento'] += $data2[$val['zone']]['rendimiento'];

  $mrow[$key]['zone'] 	 	    = $val['zone'];
  $mrow[$key]['loans'] 	 	 	  = number_format($val['loans'],0);
  $mrow[$key]['kp'] 	 	      = number_format($val['kp'],2);
  $mrow[$key]['avg'] 	 	      = number_format($val['kp']/$val['loans'],2);
  $mrow[$key]['repayment'] 	 	= number_format($val['repayment'],2);
  $mrow[$key]['write_off']    = number_format($val['write_off'],2);
  $mrow[$key]['balance'] 	 	  = number_format($val['balance'],2);
  $mrow[$key]['rendimiento']  = number_format($data2[$val['zone']]['rendimiento'],2);

}

$mrow['total']['loans'] 	  = number_format($mrow['total']['loans'],0);
$mrow['total']['kp'] 	 	  = number_format($mrow['total']['kp'],2);
$mrow['total']['avg'] 	 	  = number_format($mrow['total']['avg'],2);
$mrow['total']['repayment']   = number_format($mrow['total']['repayment'],2);
$mrow['total']['write_off']   = number_format($mrow['total']['write_off'],2);
$mrow['total']['balance'] 	  = number_format($mrow['total']['balance'],2);
$mrow['total']['rendimiento'] = number_format($mrow['total']['rendimiento'],2);

$mrow = array_reverse($mrow);

$extra['zone']['header']	       = 'tblZones.zone';
$extra['zone']['pack']	         = '%s';
$extra['zone']['align']	         = 'right';

$extra['loans']['header']		     = 'loans';
$extra['loans']['pack']		       = '%s';
$extra['loans']['align']		     = 'right';

$extra['kp']['header']	         = 'amount_disbursed';
$extra['kp']['pack']	           = '%s';
$extra['kp']['align']	           = 'right';

$extra['avg']['header']	         = 'loan_amount_average';
$extra['avg']['pack']	           = '%s';
$extra['avg']['align']	         = 'right';

$extra['repayment']['header']	   = 'principalRepayment';
$extra['repayment']['pack']		   = '%s';
$extra['repayment']['align']	   = 'right';
  
$extra['write_off']['header']	   = 'writeOff_amount';
$extra['write_off']['pack']	     = '%s';
$extra['write_off']['align']	   = 'right';

$extra['balance']['header']	     = 'portfolio_balance';
$extra['balance']['pack']		     = '%s';
$extra['balance']['align']		   = 'right';

$extra['rendimiento']['header']	 = 'operationalIncome';
$extra['rendimiento']['pack']		 = '%s';
$extra['rendimiento']['align']	 = 'right';

$extra['param']['ref']           = WEBPAGE::$scr_name;
$extra['param']['ts']            = microtime(true);

array_unshift($mrow,$extra);

WEBPAGE::makecachefile($mrow,WEBPAGE::$queryCache.'.'.$extra['param']['ts']);
require_once './includes/BS.SCR.pager.inc';

?>