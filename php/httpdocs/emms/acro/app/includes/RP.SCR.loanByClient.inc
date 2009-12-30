<?php
$date_from = $_REQUEST['date_from'] ? $_REQUEST['date_from'] : date('Y-m-d');
$date_to   = $_REQUEST['date_to']   ? $_REQUEST['date_to']   : date('Y-m-d');

$data = WEBPAGE::$dbh->getAll(sprintf("
          select
            l.loan_code,
            if(vl.id,'x','') nloan,
            concat(c.last,', ',c.first) client,
            c.code ccode,
            if(lm.borrower_type != 'I',s.name,'') society,
            lt.description,
            bt.type,
            l.kp,
            l.delivered_date,
            z.zone,
            p.program,
            f.fund,
            lm.check_number,
            u.username
          from
          (
            tblLoans l,
            tblClients c,
            tblLoanTypes lt,
            tblUsers u,
            tblZones z,
            tblPrograms p,
            tblLoansMasterDetails lmd,
            tblFundsLoansMasterPct flmp,
            tblLoansMaster lm,
            tblFunds f
          )
          left join
            view_loan_1st_time vl on vl.id = l.id
          left join
            tblBusiness b on b.id = l.business_id
          left join
            tblBusinessTypes bt on bt.id = b.type_id
          left join
            tblSocieties s on s.id = lm.borrower_id
          where
            l.delivered_date >= '%s'           and
            l.delivered_date <= '%s'           and
            c.id              = l.client_id    and
            lt.id             = l.loan_type_id and
            u.id              = l.advisor_id   and
            z.id              = l.zone_id      and
            p.id              = l.program_id   and
            lmd.loan_id       = l.id           and
            flmp.master_id    = lmd.master_id  and
            f.id              = flmp.fund_id   and
            lm.id             = lmd.master_id", $date_from, $date_to));

$head = array
        (
          'loan_code'      => WEBPAGE::$gt['tblLoans.loan_code'],
          'nloan'          => WEBPAGE::$gt['1sttimeloan'],
          'client'         => WEBPAGE::$gt['name'],
          'ccode'          => WEBPAGE::$gt['tblClients.code'],
          'society'        => WEBPAGE::$gt['tblClients.society_id'],
          'description'    => WEBPAGE::$gt['tblLoanTypes.description'],
          'type'           => WEBPAGE::$gt['tblBusinessTypes.type'],
          'kp'             => WEBPAGE::$gt['tblLoans.kp'],
          'delivered_date' => WEBPAGE::$gt['tblLoans.delivered_date'],
          'zone'           => WEBPAGE::$gt['tblZones.zone'],
          'program'        => WEBPAGE::$gt['tblPrograms.program'],
          'fund'           => WEBPAGE::$gt['tblFunds.fund'],
          'check_number'   => WEBPAGE::$gt['tblLoansMaster.check_number'],
          'username'       => WEBPAGE::$gt['tblClients.advisor_id']
        );

$tpl->setVariable('rpt_label',          WEBPAGE::$gt['loanPortfolio']);
$tpl->setVariable('rpt_subtitle_label', WEBPAGE::$gt['RP.SCR.loanByClient']);
$tpl->setVariable('date_range_label',   WEBPAGE::$gt['dateRange']);
$tpl->setVariable('date_range',         sprintf('%s : %s',$date_from,$date_to));
$tpl->setVariable('chart',              count($data) ? WEBPAGE::printchart($data,$head) : WEBPAGE::$gt['noData']);

$tpl->setVariable('releasedInPeriod',   WEBPAGE::$gt['releasedInPeriod']);

?>
