<?php
$date = $_REQUEST['date'] ? $_REQUEST['date'] : date('Y-m-d');

$data = WEBPAGE::$dbh->getAll(sprintf("
		 select s.name as society,
		 		count(lod.id) as loans, sum(lod.hits) as hits, sum(lod.delay) as delay,
		 		sum(lod.pmt) as pmt, sum(lod.penalties) as penalties, sum(lod.interest) as interest, sum(lod.fees) as fees, sum(lod.insurances) as insurances,  sum(lod.principal) as principal,
		 		u.username as advisor, z.zone as zone, p.program as program
		 		from tblLoansOnDelinquency as lod, tblClients as c, tblLoans as l, tblUsers as u, tblZones as z, tblPrograms as p, tblLoanTypes as lt, tblSocieties as s
                                where lt.id = l.loan_type_id and lt.borrower_type = 'B' and l.id = lod.loan_id and c.id = l.client_id and u.id = l.advisor_id and z.id = l.zone_id and p.id = l.program_id and s.id = c.society_id and lod.date = '%s' group by s.id", $date));

$head = array('society'=>WEBPAGE::$gt['tblSocieties.name'],'loans'=>WEBPAGE::$gt['loans'],'hits'=>WEBPAGE::$gt['payments'],'delay'=>WEBPAGE::$gt['delay'],'pmt'=>WEBPAGE::$gt['kp'],'penalties'=>WEBPAGE::$gt['PN'],'interest'=>WEBPAGE::$gt['RT'],'fees'=>WEBPAGE::$gt['FEES'],'insurances'=>WEBPAGE::$gt['INS'],'principal'=>WEBPAGE::$gt['KP'],'advisor'=>WEBPAGE::$gt['tblClients.advisor_id'],'zone'=>WEBPAGE::$gt['tblZones.zone'],'program'=>WEBPAGE::$gt['tblPrograms.program']);

$total = current(WEBPAGE::$dbh->getAll(sprintf("
		 select '%s' as society,
		 		count(lod.id) as loans, sum(lod.hits) as hits, sum(lod.delay) as delay,
		 		sum(lod.pmt) as pmt, sum(lod.penalties) as penalties, sum(lod.interest) as interest, sum(lod.fees) as fees, sum(lod.insurances) as insurances,  sum(lod.principal) as principal,
		 		'-' as advisor, '-' as zone, '-' as program
		 		from tblLoansOnDelinquency as lod, tblLoans as l, tblLoanTypes as lt where lt.id = l.loan_type_id and lt.borrower_type = 'B' and l.id = lod.loan_id and lod.date = '%s'", WEBPAGE::$gt['total'],$date)));

foreach($data as $key=>$val) {
  $data[$key]['society'] 	 	= str_replace(' ','&nbsp;',$data[$key]['society']);
  $data[$key]['zone'] 	 	 	= str_replace(' ','&nbsp;',$data[$key]['zone']);
  $data[$key]['program'] 	 	= str_replace(' ','&nbsp;',$data[$key]['program']);
  }

if(count($data)) { $data['total'] = $total; }
$tpl->setVariable('rpt_label', 		WEBPAGE::$gt['delinquencyPortfolio']);
$tpl->setVariable('rpt_subtitle_label', WEBPAGE::$gt['RP.SCR.delinquencyByGroup']);
$tpl->setVariable('data_date_label', 	WEBPAGE::$gt['date']);
$tpl->setVariable('data_date', 		$date);
$tpl->setVariable('chart', count($data) ? WEBPAGE::printchart($data,$head) : WEBPAGE::$gt['noData']);
?>