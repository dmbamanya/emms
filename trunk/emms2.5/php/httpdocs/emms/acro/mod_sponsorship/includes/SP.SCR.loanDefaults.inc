<?php

$show_gohome = true;

$data = WEBPAGE::$dbh->getAll("select * from tblLoanTypes");

foreach($data2 as $key=>$val)
{
  $mrow[$key]['borrower_type']     = WEBPAGE::$gt[sprintf('tblLoanTypes.borrower_type.%s',$val['borrower_type'])];
  $mrow[$key]['installment']       = $val['installment'];
  $mrow[$key]['payment_frequency'] = WEBPAGE::$gt[sprintf('tblLoanTypes.payment_frequency.%s',$val['payment_frequency'])];
  $mrow[$key]['rates_r']           = $val['rates_r'];
  $mrow[$key]['rates_d']           = $val['rates_d'];
  $mrow[$key]['fees_af']           = $val['fees_af'];
  $mrow[$key]['fees_at']           = $val['fees_at'];
  $mrow[$key]['margin_d']          = $val['margin_d'];
}

foreach($data as $key=>$val)
{
  $mrow[$key]['description']       = GETTEXT::_($val['description']);
  $mrow[$key]['borrower_type']     = WEBPAGE::$gt[sprintf('tblLoanTypes.borrower_type.%s',$val['borrower_type'])];
  $mrow[$key]['installment']       = $val['installment'];
  $mrow[$key]['payment_frequency'] = WEBPAGE::$gt[sprintf('tblLoanTypes.payment_frequency.%s',$val['payment_frequency'])];
  $mrow[$key]['rates_r']           = $val['rates_r'];
  $mrow[$key]['fees_af']           = $val['fees_af'];
  $mrow[$key]['fees_at']           = $val['fees_at'];
}

$mrow = array_reverse($mrow);

$extra['description']['header']	      = 'tblLoanTypes.description';
$extra['description']['pack']	        = '%s';
$extra['description']['align']	      = 'right';

$extra['borrower_type']['header']		  = 'tblLoanTypes.borrower_type';
$extra['borrower_type']['pack']		    = '%s';
$extra['borrower_type']['align']		  = 'right';

$extra['installment']['header']	      = 'tblLoanTypes.installment';
$extra['installment']['pack']	        = '%s';
$extra['installment']['align']	      = 'right';

$extra['payment_frequency']['header'] = 'tblLoanTypes.payment_frequency';
$extra['payment_frequency']['pack']	  = '%s';
$extra['payment_frequency']['align']  = 'right';

$extra['rates_r']['header']	          = 'tblLoanTypes.rates_r';
$extra['rates_r']['pack']		          = '%s';
$extra['rates_r']['align']	          = 'right';

$extra['fees_af']['header']	          = 'tblLoanTypes.fees_af';
$extra['fees_af']['pack']	            = '%s';
$extra['fees_af']['align']	          = 'right';

$extra['fees_at']['header']	          = 'tblLoanTypes.fees_at';
$extra['fees_at']['pack']		          = '%s';
$extra['fees_at']['align']	          = 'right';

$extra['param']['ref']                = WEBPAGE::$scr_name;
$extra['param']['ts']                 = microtime(true);

array_unshift($mrow,$extra);

WEBPAGE::makecachefile($mrow,WEBPAGE::$queryCache.'.'.$extra['param']['ts']);
require_once './includes/BS.SCR.pager.inc';

?>