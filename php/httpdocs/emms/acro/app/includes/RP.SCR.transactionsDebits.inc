<?php
$date_from 	= $_REQUEST['date_from'] 	? $_REQUEST['date_from'] 	: date('Y-m-d');
$date_to 	= $_REQUEST['date_to'] 		? $_REQUEST['date_to'] 		: date('Y-m-d');
$tpl->setVariable('dateRangeSelector', 	WEBPAGE::$gt['BS.SCR.dateRangeSelector']);
$tpl->setVariable('oldrange', 			sprintf('%s - %s',$date_from,$date_to));

$data = WEBPAGE::$dbh->getAll(sprintf("
		SELECT  d.date   ,
        z.zone   ,
        p.program,
        f.fund   ,
        d.amount ,
        d.code
FROM    tblTDebits  AS d           ,
        tblZones    AS z           ,
        tblPrograms AS p           ,
        tblFundsLoansMasterPct flmp,
        tblFunds f
WHERE   z.id           = d.branch_id
    AND p.id           = d.program_id
    AND flmp.master_id = d.id
    AND f.id           = flmp.fund_id
    AND d.date        >= '%s'
    AND d.date        <= '%s'
ORDER BY d.code,d.branch_id,d.date,
        d.program_id", $date_from, $date_to));


$head = array('date'=>WEBPAGE::$gt['date'],'zone'=>WEBPAGE::$gt['tblZones.zone'],'program'=>WEBPAGE::$gt['tblPrograms.program'],'fund'=>WEBPAGE::$gt['tblFunds.fund'],'amount'=>WEBPAGE::$gt['tblLoans.kp'],'code'=>WEBPAGE::$gt['code']);
$tpl->setVariable('rpt_label', 			WEBPAGE::$gt['transactionsDebits']);
//$tpl->setVariable('rpt_subtitle_label', WEBPAGE::$gt['transactionsDebits']);
$tpl->setVariable('date_range_label', 	WEBPAGE::$gt['dateRange']);
$tpl->setVariable('date_range', 		sprintf('%s : %s',$date_from,$date_to));
$tpl->setVariable('chart', count($data) ? WEBPAGE::printchart($data,$head) : WEBPAGE::$gt['noData']);

?>
