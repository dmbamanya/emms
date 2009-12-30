<?php

$date = $_REQUEST['date'] ? $_REQUEST['date'] : date('Y-m-d');

$data = WEBPAGE::$dbh->getAll(sprintf("
		 select
            xl.loan_code, c.code, concat(c.last,', ', c.first) client, if((lm.borrower_type != 'I'),s.name,'-') bde, xl.kp, u.username, f.fund, p.program, z.zone, xl.date_g, xl.date_c, xl.status
         from
            (
            select
                lsh1.loan_id,lsh1.date date_g,
                lsh2.date date_c,
                l.loan_code,l.client_id,l.program_id,l.zone_id, l.advisor_id, l.kp, l.status,
                lmd.master_id
            from
                tblLoans l, tblLoansMasterDetails lmd, tblLoanStatusHistory lsh1
            left join
                tblLoanStatusHistory lsh2 on lsh2.loan_id = lsh1.loan_id and lsh2.p_status = 'G'
            where
                lsh1.status = 'G' and
                lsh1.date <= '%s' and
                ((lsh2.date >= '%s')or(lsh2.date is null)) and
                l.id = lsh1.loan_id and
                lmd.loan_id = lsh1.loan_id
            )
            xl, tblClients c, tblPrograms p, tblZones z, tblUsers u, tblFundsLoansMasterPct flmp, tblFunds f, tblLoansMaster lm
        left join
            tblSocieties s on s.id = lm.borrower_id
        where
            c.id = xl.client_id and
            lm.id = xl.master_id and
            z.id = xl.zone_id and
            p.id = xl.program_id and
            u.id = xl.advisor_id and
            flmp.master_id = xl.master_id and
            f.id = flmp.fund_id", $date, $date));

$head = array('loan_code' => WEBPAGE::$gt['tblLoans.loan_code'], 'code' => WEBPAGE::$gt['tblClients.code'], 'client' => WEBPAGE::$gt['tblClients.id'], 'bde' => WEBPAGE::$gt['tblClients.society_id'], 'kp' => WEBPAGE::$gt['tblLoans.kp'],'username' => WEBPAGE::$gt['tblClients.advisor_id'], 'fund' => WEBPAGE::$gt['tblFunds.fund'], 'program' => WEBPAGE::$gt['tblPrograms.program'], 'zone' => WEBPAGE::$gt['tblZones.zone'], 'dateG' => WEBPAGE::$gt['tblLoans.delivered_date'], 'dateC' => WEBPAGE::$gt['cancel_date'], 'status' => WEBPAGE::$gt['tblLoans.status']);
$pack = array('status' => 'tblLoans.status.%s');

$tpl->setVariable('rpt_label', 			WEBPAGE::$gt['activeLoanPortfolio']);
$tpl->setVariable('rpt_subtitle_label', WEBPAGE::$gt['RP.SCR.activeLoanPortfolioByLoan']);
$tpl->setVariable('data_date_label', 	WEBPAGE::$gt['date']);
$tpl->setVariable('data_date', 			$date);
$tpl->setVariable('chart', count($data) ? WEBPAGE::printchart($data,$head,$pack) : WEBPAGE::$gt['noData']);
?>


