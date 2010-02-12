<?php

$date = $_REQUEST['date'] ? $_REQUEST['date'] : date('Y-m-d');

$data = WEBPAGE::$dbh->getAll(sprintf("
	select
            xl.loan_id, 
            xl.loan_code,
            bst.type business_type,
            c.code,
            concat(c.last, ', ', c.first) client,
            if((lm.borrower_type != 'I'),s.name,'-') bde,
            xl.kp,
            u.username,
            f.fund,
            p.program,
            z.zone,
            xl.date_g,
            xl.date_c,
            xl.status,
            coalesce(sum(vrf.flag_a) - count(vrf.flag_a),0) flag_a,
            coalesce(sum(vrf.flag_b) - count(vrf.flag_b),0) flag_b
         from
         (
            (
            select
                lsh1.loan_id,lsh1.date date_g,
                lsh2.date date_c,
                l.loan_code,l.client_id,l.program_id,l.zone_id, l.advisor_id, l.kp, l.status,
                bs.type_id,
                lmd.master_id
            from
                (tblLoans l, tblLoansMasterDetails lmd, tblLoanStatusHistory lsh1)
            left join
                tblLoanStatusHistory lsh2 on lsh2.loan_id = lsh1.loan_id and lsh2.p_status = 'G'
            left join
                tblBusiness bs on bs.id = l.business_id
            where
                lsh1.status = 'G' and
                lsh1.date <= '%s' and
                ((lsh2.date >= '%s')or(lsh2.date is null)) and
                l.id = lsh1.loan_id and
                lmd.loan_id = lsh1.loan_id
            )
            xl, tblClients c, tblPrograms p, tblZones z, tblUsers u, tblFundsLoansMasterPct flmp, tblFunds f, tblLoansMaster lm
        )
        left join
            tblSocieties s on s.id = lm.borrower_id
        left join
            tblBusinessTypes bst on bst.id = xl.type_id
        left join
            view_receipt_flags vrf on vrf.loan_id = xl.loan_id and vrf.date <= '%s'
        where
            c.id = xl.client_id and
            lm.id = xl.master_id and
            z.id = xl.zone_id and
            p.id = xl.program_id and
            u.id = xl.advisor_id and
            flmp.master_id = xl.master_id and
            f.id = flmp.fund_id
        group by
            xl.loan_id", $date, $date, $date));

$head = array
        (
          'loan_code'     => WEBPAGE::$gt['tblLoans.loan_code'],
          'business_type' => WEBPAGE::$gt['tblBusinessTypes.type'],
          'code'          => WEBPAGE::$gt['tblClients.code'],
          'client'        => WEBPAGE::$gt['tblClients.id'],
          'bde'           => WEBPAGE::$gt['tblClients.society_id'],
          'kp'            => WEBPAGE::$gt['tblLoans.kp'],
          'username'      => WEBPAGE::$gt['tblClients.advisor_id'],
          'fund'          => WEBPAGE::$gt['tblFunds.fund'],
          'program'       => WEBPAGE::$gt['tblPrograms.program'],
          'zone'          => WEBPAGE::$gt['tblZones.zone'],
          'date_g'        => WEBPAGE::$gt['tblLoans.delivered_date'],
          'date_c'        => WEBPAGE::$gt['cancel_date'],
          'status'        => WEBPAGE::$gt['tblLoans.status'],
          'flag_a'        => WEBPAGE::$conf['app']['pmt_receipt_flag_a'],
          'flag_b'        => WEBPAGE::$conf['app']['pmt_receipt_flag_b']
        );

$pack = array
        (
          'status'    => 'tblLoans.status.%s'
        );

if (!WEBPAGE::$conf['app']['pmt_receipt_flag_a'])
{
    unset($head['flag_a']);
}

if (!WEBPAGE::$conf['app']['pmt_receipt_flag_b'])
{
    unset($head['flag_b']);
}

$tpl->setVariable('rpt_label',          WEBPAGE::$gt['activeLoanPortfolio']);
$tpl->setVariable('rpt_subtitle_label', WEBPAGE::$gt['RP.SCR.activeLoanPortfolioByLoan']);
$tpl->setVariable('data_date_label', 	WEBPAGE::$gt['date']);
$tpl->setVariable('data_date',          $date);
$tpl->setVariable('chart', count($data) ? WEBPAGE::printchart($data,$head,$pack) : WEBPAGE::$gt['noData']);
?>


