<?php
$date = $_REQUEST['date'] ? $_REQUEST['date'] : date('Y-m-d');

$data = WEBPAGE::$dbh->getAll(sprintf("
						select
    						l.client_id as client_id,
    						concat(c.last,', ',c.first) as client,
    						c.code as ccode,
    						(if(isnull(c.phone) or c.phone=' ',c.mobile,c.phone)) as phone,
    						s.name as name,
    						count(lod.id) as loans,
    						sum(lod.hits) as hits,
    						sum(lod.delay) as delay,
    						sum(lod.pmt) as pmt,
    						sum(lod.penalties) as penalties,
    						sum(lod.interest) as interest,
    						sum(lod.fees) as fees,
    						sum(lod.insurances) as insurances,
    						sum(lod.principal) as principal,
    						u.username as advisor,
    						z.zone as zone,
    						p.program as program,
    						f.fund as fund
  						from
    						(
    						tblLoansOnDelinquency as lod,
    						tblClients as c,
    						tblLoans as l,
    						tblUsers as u,
    						tblZones as z,
    						tblPrograms as p,
    						tblLoanTypes as lt,
    						tblLoansMasterDetails as lmd,
    						tblFundsLoansMasterPct as flmp,
    						tblFunds as f
    						)
    						LEFT JOIN tblSocieties as s ON c.society_id = s.id
  						where
    						lt.id = l.loan_type_id and
    						lt.borrower_type != 'G' and
    						l.id = lod.loan_id and
    						c.id = l.client_id and
    						u.id = l.advisor_id and
    						z.id = l.zone_id and
    						p.id = l.program_id and
    						lmd.loan_id = lod.loan_id and
    						flmp.master_id = lmd.master_id and
    						f.id = flmp.fund_id and
    						lod.date = '%s'
  						group by
    						l.client_id
  						order by
    						s.name,u.username", $date));


$total = current(WEBPAGE::$dbh->getAll(sprintf("
		 select '%s' as client, '-' as ccode,'-' as phone, '-' as name,
		 		count(lod.id) as loans, sum(lod.hits) as hits, sum(lod.delay) as delay,
		 		sum(lod.pmt) as pmt, sum(lod.penalties) as penalties, sum(lod.interest) as interest, sum(lod.fees) as fees, sum(lod.insurances) as insurances,  sum(lod.principal) as principal,
		 		'-' as advisor, '-' as zone, '-' as program, '-' as fund
		 		from tblLoansOnDelinquency as lod, tblLoans as l, tblLoanTypes as lt where lt.id = l.loan_type_id and lt.borrower_type != 'G' and l.id = lod.loan_id and lod.date = '%s'", WEBPAGE::$gt['total'],$date)));



$head = array('client'=>WEBPAGE::$gt['tblClients.id'],'ccode'=>WEBPAGE::$gt['tblClients.code'],'phone'=>WEBPAGE::$gt['tblClients.phone'],'name'=>WEBPAGE::$gt['tblSocieties.name'],'loans'=>WEBPAGE::$gt['loans'],'hits'=>WEBPAGE::$gt['payments'],'delay'=>WEBPAGE::$gt['delay'],'pmt'=>WEBPAGE::$gt['kp'],'penalties'=>WEBPAGE::$gt['PN'],'interest'=>WEBPAGE::$gt['RT'],'fees'=>WEBPAGE::$gt['FEES'],'insurances'=>WEBPAGE::$gt['INS'],'principal'=>WEBPAGE::$gt['KP'],'advisor'=>WEBPAGE::$gt['tblClients.advisor_id'],'zone'=>WEBPAGE::$gt['tblZones.zone'],'program'=>WEBPAGE::$gt['tblPrograms.program'],'fund'=>WEBPAGE::$gt['tblFunds.fund']);




foreach($data as $key=>$val) {
  unset($data[$key]['client_id']);
//  $client->data['id'] = $val['client_id'];
//  $client->checkConfidentiality();
//  if ($client->data['confidential']) {
//    $data[$key]['client'] = $client->data['name'];
//    $data[$key]['ccode']  = $client->data['code'];
//    }
  $data[$key]['client'] 	 	= str_replace(' ','&nbsp;',$data[$key]['client']);
  $data[$key]['zone'] 	 	 	= str_replace(' ','&nbsp;',$data[$key]['zone']);
  $data[$key]['program'] 	 	= str_replace(' ','&nbsp;',$data[$key]['program']);
  $data[$key]['fund'] 	 	    = str_replace(' ','&nbsp;',$data[$key]['fund']);
  }



if(count($data)) { $data['total'] = $total; }

$tpl->setVariable('rpt_label', 		WEBPAGE::$gt['delinquencyPortfolio']);
$tpl->setVariable('rpt_subtitle_label', WEBPAGE::$gt['RP.SCR.delinquencyByClient']);
$tpl->setVariable('data_date_label', 	WEBPAGE::$gt['date']);
$tpl->setVariable('data_date', 		$date);
$tpl->setVariable('chart', count($data) ? WEBPAGE::printchart($data,$head) : WEBPAGE::$gt['noData']);
?>