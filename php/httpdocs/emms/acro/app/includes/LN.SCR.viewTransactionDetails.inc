<?php

  $transac = WEBPAGE::$dbh->getAll(sprintf("select tc.date, z.zone, p.program from tblTCredits as tc, tblZones as z, tblPrograms as p where p.id = tc.program_id and z.id = tc.branch_id and tc.code = '%s'", $_REQUEST['transaction_id']));
  $mrow = WEBPAGE::$dbh->getAll(sprintf("
                                    select
                                        l.loan_code,
                                        concat(c.first, ' ', c.last) as client,
                                        if((lt.borrower_type != 'I'),s.name,'-') bde,
                                        p.pmt, p.principal, p.insurances, p.fees, p.interest, p.penalties, p.balance_kp,
                                        if(p.special,'%s','%s') as special,
                                        u.username
                                    from
                                        tblLoans as l,
                                        tblClients as c,
                                        tblPayments as p,
                                        tblLoanTypes as lt,
                                        tblUsers as u,
                                        tblLoansMasterDetails lmd,
                                        tblLoansMaster lm
                                    left join
                                        tblSocieties s on s.id = lm.borrower_id
                                    where
                                        u.id = p.user_id and
                                        p.transaction_id = '%s' and
                                        l.id = p.loan_id and
                                        c.id = l.client_id and
                                        lt.id = l.loan_type_id and
                                        lmd.loan_id = p.loan_id and
                                        lm.id = lmd.master_id
                                    order by
                                        client,p.id",WEBPAGE::$gt['yes'],WEBPAGE::$gt['no'],$_REQUEST['transaction_id']));

  $head = array('loan_code'=>WEBPAGE::$gt['tblLoans.loan_code'],'client'=>WEBPAGE::$gt['tblClients.id'],'bde'=>WEBPAGE::$gt['tblClients.society_id'],'pmt'=>WEBPAGE::$gt['tblPayments.pmt'],'principal'=>WEBPAGE::$gt['tblPayments.principal'],'insurances'=>WEBPAGE::$gt['tblPayments.insurances'],'fees'=>WEBPAGE::$gt['tblPayments.fees'],'interest'=>WEBPAGE::$gt['tblPayments.interest'],'penalties'=>WEBPAGE::$gt['tblPayments.penalties'],'balance_kp'=>WEBPAGE::$gt['tblPayments.balance_kp'],'special'=>WEBPAGE::$gt['tblPayments.special'],'user_id'=>WEBPAGE::$gt['tblUsers.id']);

  $total = array();
  foreach ($mrow as $key=>$val) {
    $mrow[$key]['client'] = str_replace(' ','&nbsp;',$val['client']);
    
    $total['loan_code']		 = WEBPAGE::$gt['total'];
    $total['client']		 = '-';
    $total['bde']            = '-';
    $total['pmt'] 			+= $val['pmt'];
    $total['principal'] 	+= $val['principal'];
    $total['insurances'] 	+= $val['insurances'];
    $total['fees'] 			+= $val['fees'];
    $total['interest'] 		+= $val['interest'];
    $total['penalties'] 	+= $val['penalties'];
    $total['balance_kp']	 = '-';
    $total['special']		 = '-';
    $total['user_id']		 = '-';
    
    }
  $mrow[] = $total;

  $tpl->setVariable('chart', count($total) ? WEBPAGE::printchart($mrow,$head) : WEBPAGE::$gt['noData']);
  $tpl->setVariable('content_title',		WEBPAGE::$gt[WEBPAGE::$scr_name]);
  $tpl->setVariable('transaction_label',	WEBPAGE::$gt['tblPayments.transaction_id']);
  $tpl->setVariable('transaction_value',	$_REQUEST['transaction_id']);
  $tpl->setVariable('date_label',			WEBPAGE::$gt['date']);
  $tpl->setVariable('date_value',			$transac[0]['date']);
  $tpl->setVariable('branch_label',			WEBPAGE::$gt['tblZones.zone']);
  $tpl->setVariable('branch_value',			$transac[0]['zone']);
  $tpl->setVariable('program_label',		WEBPAGE::$gt['tblPrograms.program']);
  $tpl->setVariable('program_value',		$transac[0]['program']);

?>
