<?php
error_reporting(E_ALL);
require_once 'class/webpage.php';
require_once 'class/sql.php';
require_once 'PEAR.php';
require_once './includes/ST.LIB.login.inc';
require_once ("Mail.php");
require_once ("Mail/mime.php");

WEBPAGE::START();
WEBPAGE::$lang = isset($_GET['lang']) ? $_GET['lang'] : WEBPAGE::$conf['app']['default_language'];
WEBPAGE::load_gt();

if ($_SERVER['REMOTE_ADDR'] != WEBPAGE::$conf['server']['ip']) { exit; }

$data = WEBPAGE::$dbh->getAll(sprintf("
          select
            concat(u.first, ' ',u.last)            loan_officer      ,
            coalesce(c1.clients,0)                 clients_1         ,
            coalesce(c2.clients,0)                 clients_2         ,
            coalesce(c3.clients,0)                 clients_3         ,
            coalesce(c1.women_pct,0)               women_pct_1       ,
            coalesce(c2.women_pct,0)               women_pct_2       ,
            coalesce(c3.women_pct,0)               women_pct_3       ,
            coalesce(c1.loans,0)                   loans_1           ,
            coalesce(c2.loans,0)                   loans_2           ,
            coalesce(c3.loans,0)                   loans_3           ,
            coalesce(d1.delinquent_loans,0)        delinquent_loans_1,
            coalesce(d2.delinquent_loans,0)        delinquent_loans_2,
            coalesce(d3.delinquent_loans,0)        delinquent_loans_3,
            coalesce(d1.due_amount,0.00)           due_amount_1      ,
            coalesce(d2.due_amount,0.00)           due_amount_2      ,
            coalesce(d3.due_amount,0.00)           due_amount_3      ,
            coalesce(k1.disbursements,0.00)        disbursements_1   ,
            coalesce(k2.disbursements,0.00)        disbursements_2   ,
            coalesce(k3.disbursements,0.00)        disbursements_3   ,
            coalesce(k1.formalizations,0)          formalizations_1  ,
            coalesce(k2.formalizations,0)          formalizations_2  ,
            coalesce(k3.formalizations,0)          formalizations_3  ,
            coalesce(i1.op_income,0.00)            op_income_1       ,
            coalesce(i2.op_income,0.00)            op_income_2       ,
            coalesce(i3.op_income,0.00)            op_income_3       ,
            coalesce(i1.kp_income,0.00)            kp_income_1       ,
            coalesce(i2.kp_income,0.00)            kp_income_2       ,
            coalesce(i3.kp_income,0.00)            kp_income_3       ,
            coalesce(r1.balance,0.00)              balance_1         ,
            coalesce(r2.balance,0.00)              balance_2         ,
            coalesce(r3.balance,0.00)              balance_3         ,
            coalesce(r1.riskA,0.00)                riskA_1           ,
            coalesce(r2.riskA,0.00)                riskA_2           ,
            coalesce(r3.riskA,0.00)                riskA_3           ,
            coalesce(w1.write_off,0.00)            write_off_1       ,
            coalesce(w2.write_off,0.00)            write_off_2       ,
            coalesce(w3.write_off,0.00)            write_off_3
          from
            tblUsers u
          left join
            (
              select
                cp.advisor_id     advisor,
                sum(cp.clients)   clients,
                100*sum(cp.female)/sum(cp.clients)    women_pct,
                sum(cp.client_al) loans
              from
                tblClientPortfolio cp
              where
                cp.date = CURDATE() - INTERVAL 30 DAY
              group by
                cp.advisor_id
            ) c1 on c1.advisor = u.id
          left join
            (
              select
                cp.advisor_id     advisor,
                sum(cp.clients)   clients,
                100*sum(cp.female)/sum(cp.clients)    women_pct,
                sum(cp.client_al) loans
              from
                tblClientPortfolio cp
              where
                cp.date = CURDATE() - INTERVAL 7 DAY
              group by
                cp.advisor_id
            ) c2 on c2.advisor = u.id
          left join
            (
              select
                cp.advisor_id     advisor,
                sum(cp.clients)   clients,
                100*sum(cp.female)/sum(cp.clients)    women_pct,
                sum(cp.client_al) loans
              from
                tblClientPortfolio cp
              where
                cp.date = CURDATE()
              group by
                cp.advisor_id
            ) c3 on c3.advisor = u.id
          left join
            (
              select
                l.advisor_id   advisor         ,
                count(lod.id)  delinquent_loans,
                sum(lod.hits)  due_payments    ,
                sum(lod.pmt)   due_amount
              from
                tblLoans               l,
                tblLoansOnDelinquency lod
              where
                lod.date = CURDATE() - INTERVAL 30 DAY and
                l.id = lod.loan_id
              group by
                l.advisor_id
            ) d1 on d1.advisor = u.id
          left join
            (
              select
                l.advisor_id   advisor         ,
                count(lod.id)  delinquent_loans,
                sum(lod.hits)  due_payments    ,
                sum(lod.pmt)   due_amount
              from
                tblLoans               l,
                tblLoansOnDelinquency lod
              where
                lod.date = CURDATE() - INTERVAL 7 DAY and
                l.id = lod.loan_id
              group by
                l.advisor_id
            ) d2 on d2.advisor = u.id
          left join
            (
              select
                l.advisor_id   advisor         ,
                count(lod.id)  delinquent_loans,
                sum(lod.hits)  due_payments    ,
                sum(lod.pmt)   due_amount
              from
                tblLoans               l,
                tblLoansOnDelinquency lod
              where
                lod.date = CURDATE() and
                l.id = lod.loan_id
              group by
                l.advisor_id
            ) d3 on d3.advisor = u.id
          left join
            (
              select
                l.advisor_id   advisor,
                sum(l.kp)      disbursements,
                count(l.id)    formalizations
              from
                tblLoans               l,
                tblLoanStatusHistory lsh
              where
                lsh.status = 'G' and
                lsh.date >= CURDATE() - INTERVAL 30 DAY and
                lsh.date <  CURDATE()                  and
                l.id = lsh.loan_id
              group by
                l.advisor_id
            ) k1 on k1.advisor = u.id
          left join
            (
              select
                l.advisor_id   advisor,
                sum(l.kp)      disbursements,
                count(l.id)    formalizations
              from
                tblLoans               l,
                tblLoanStatusHistory lsh
              where
                lsh.status = 'G' and
                lsh.date >= CURDATE() - INTERVAL 7 DAY and
                lsh.date <  CURDATE()                  and
                l.id = lsh.loan_id
              group by
                l.advisor_id
            ) k2 on k2.advisor = u.id
          left join
            (
              select
                l.advisor_id   advisor,
                sum(l.kp)      disbursements,
                count(l.id)    formalizations
              from
                tblLoans               l,
                tblLoanStatusHistory lsh
              where
                lsh.status = 'G' and
                lsh.date = CURDATE() - INTERVAL 1 DAY and
                l.id = lsh.loan_id
              group by
                l.advisor_id
            ) k3 on k3.advisor = u.id
          left join
            (
              select
                l.advisor_id                                       advisor  ,
                sum(p.interest) + sum(p.insurances) + sum(p.fees)  op_income,
                sum(p.principal)                                   kp_income
              from
                tblLoans    l,
                tblPayments p
              where
                p.date >= CURDATE() - INTERVAL 30 DAY and
                p.date <= CURDATE()                  and
                l.id = p.loan_id
              group by
                l.advisor_id
            ) i1 on i1.advisor = u.id
          left join
            (
              select
                l.advisor_id                                       advisor  ,
                sum(p.interest) + sum(p.insurances) + sum(p.fees)  op_income,
                sum(p.principal)                                   kp_income
              from
                tblLoans    l,
                tblPayments p
              where
                p.date >= CURDATE() - INTERVAL 7 DAY and
                p.date <= CURDATE()                  and
                l.id = p.loan_id
              group by
                l.advisor_id
            ) i2 on i2.advisor = u.id
          left join
            (
              select
                l.advisor_id                                       advisor  ,
                sum(p.interest) + sum(p.insurances) + sum(p.fees)  op_income,
                sum(p.principal)                                   kp_income
              from
                tblLoans    l,
                tblPayments p
              where
                p.date >= CURDATE() - INTERVAL 1 DAY and
                p.date <= CURDATE()                  and
                l.id = p.loan_id
              group by
                l.advisor_id
            ) i3 on i3.advisor = u.id
          left join
            (
              select
                rp.advisor_id                                advisor,
                round(sum(rp.balance),2)                     balance,
                round(100*(sum(rp.riskA)/sum(rp.balance)),2) riskA
              from
                tblRiskPortfolio rp
              where
                rp.date = CURDATE() - INTERVAL 30 DAY
              group by
                rp.advisor_id
            ) r1 on r1.advisor = u.id
          left join
            (
              select
                rp.advisor_id                                advisor,
                round(sum(rp.balance),2)                     balance,
                round(100*(sum(rp.riskA)/sum(rp.balance)),2) riskA
              from
                tblRiskPortfolio rp
              where
                rp.date = CURDATE() - INTERVAL 7 DAY
              group by
                rp.advisor_id
            ) r2 on r2.advisor = u.id
          left join
            (
             select
                rp.advisor_id                                advisor,
                round(sum(rp.balance),2)                     balance,
                round(100*(sum(rp.riskA)/sum(rp.balance)),2) riskA
              from
                tblRiskPortfolio rp
              where
                rp.date = CURDATE()
              group by
                rp.advisor_id
            ) r3 on r3.advisor = u.id
          left join
            (
              select
                l.advisor_id            advisor,
                round(sum(wo.amount),2) write_off
              from
                tblLoanWriteOff wo,
                tblLoans         l
              where
                wo.date >= CURDATE() - INTERVAL 30 DAY and
                wo.date <= CURDATE()                   and
                l.id = wo.loan_id
              group by
                l.advisor_id
            ) w1 on w1.advisor = u.id
          left join
            (
              select
                l.advisor_id            advisor,
                round(sum(wo.amount),2) write_off
              from
                tblLoanWriteOff wo,
                tblLoans         l
              where
                wo.date >= CURDATE() - INTERVAL 7 DAY and
                wo.date <= CURDATE()                  and
                l.id = wo.loan_id
              group by
                l.advisor_id
            ) w2 on w2.advisor = u.id
          left join
            (
              select
                l.advisor_id            advisor,
                round(sum(wo.amount),2) write_off
              from
                tblLoanWriteOff wo,
                tblLoans         l
              where
                wo.date = CURDATE() and
                l.id = wo.loan_id
              group by
                l.advisor_id
            ) w3 on w3.advisor = u.id
          where
            (r1.balance or r2.balance or r3.balance) is not null
          order by
            u.zone_id, u.first"));

$_html .= <<<CHTML

<head>
<title>Loan officer stats</title>
</head>
<body>

CHTML;

$_html .= "<table border=0 cellpadding=0px cellspacing=0px bgcolor=white>";
$_html .= "<tr align=center style='background-color:#222244; color: white; font: small-caps bold 14px verdana;'>
             <td style='padding: 5px;'>Loan Officer</td>
             <td style='padding: 5px;'>Score</td>
             <td style='padding: 5px;'>Prest.</td>
             <td style='padding: 5px;' width=80px>Prest. atraso</td>
             <td style='padding: 5px;' width=80px>Monto atraso</td>
             <td style='padding: 5px;'>Clients</td>
             <td style='padding: 5px;'>Women Pct</td>
             <td style='padding: 5px;'>Balance</td>
             <td style='padding: 5px;'>PAR 15</td>
             <td style='padding: 5px;'>W_Off Pct</td>
             <td style='padding: 5px;'>Op. Income</td>
             <td style='padding: 5px;'>Formalizations</td>
             <td style='padding: 5px;'>Disbursements</td>
             <td style='padding: 5px;'>Write Off</td>
             <td style='padding: 5px;'>Kp. Income</td>
           </tr>";
foreach($data as $key => $val)
{
    $score  = fscore($val['clients_3'], 'clients') +
              fscore($val['women_pct_3'], 'women_pct')  +
              fscore($val['balance_3'], 'balance') +
              fscore($val['riskA_3'], 'riskA') +
              fscore(100*($val['write_off_1']/$val['balance_1']), 'write_off_pct') +
              fscore($val['op_income_1'], 'op_income') +
              fscore($val['formalizations_1'], 'formalizations') +
              fscore($val['disbursements_1'], 'disbursements');
    
    $_html .= sprintf("
                <tr><td style='height:1px;'></td></tr>
                <tr style='background-color:white; padding: 0px;'>
                  <td bgcolor=%s>%s</td>
                  <td bgcolor=%s align=center><font size=6>%s</font>&nbsp;</td>
                  <td><table cellpadding=5px cellspacing=0 align=right style='text-align: right; width:100%%;'><tr><td bgcolor=%s>%s</td></tr><tr><td bgcolor=%s>%s</td></tr><tr><td bgcolor=%s>%s</td></tr></table></td>
                  <td><table cellpadding=5px cellspacing=0 align=right style='text-align: right; width:100%%;'><tr><td bgcolor=%s>%s</td></tr><tr><td bgcolor=%s>%s</td></tr><tr><td bgcolor=%s>%s</td></tr></table></td>
                  <td><table cellpadding=5px cellspacing=0 align=right style='text-align: right; width:100%%;'><tr><td bgcolor=%s>%s</td></tr><tr><td bgcolor=%s>%s</td></tr><tr><td bgcolor=%s>%s</td></tr></table></td>
                  <td><table cellpadding=5px cellspacing=0 align=right style='text-align: right; width:100%%;'><tr><td bgcolor=%s>%s</td></tr><tr><td bgcolor=%s>%s</td></tr><tr><td bgcolor=%s>%s</td></tr></table></td>
                  <td><table cellpadding=5px cellspacing=0 align=right style='text-align: right; width:100%%;'><tr><td bgcolor=%s>%s</td></tr><tr><td bgcolor=%s>%s</td></tr><tr><td bgcolor=%s>%s</td></tr></table></td>
                  <td><table cellpadding=5px cellspacing=0 align=right style='text-align: right; width:100%%;'><tr><td bgcolor=%s>%s</td></tr><tr><td bgcolor=%s>%s</td></tr><tr><td bgcolor=%s>%s</td></tr></table></td>
                  <td><table cellpadding=5px cellspacing=0 align=right style='text-align: right; width:100%%;'><tr><td bgcolor=%s>%s</td></tr><tr><td bgcolor=%s>%s</td></tr><tr><td bgcolor=%s>%s</td></tr></table></td>
                  <td><table cellpadding=5px cellspacing=0 align=right style='text-align: right; width:100%%;'><tr><td bgcolor=%s>%s</td></tr><tr><td bgcolor=%s>%s</td></tr><tr><td bgcolor=%s>%s</td></tr></table></td>
                  <td><table cellpadding=5px cellspacing=0 align=right style='text-align: right; width:100%%;'><tr><td bgcolor=%s>%s</td></tr><tr><td bgcolor=%s>%s</td></tr><tr><td bgcolor=%s>%s</td></tr></table></td>
                  <td><table cellpadding=5px cellspacing=0 align=right style='text-align: right; width:100%%;'><tr><td bgcolor=%s>%s</td></tr><tr><td bgcolor=%s>%s</td></tr><tr><td bgcolor=%s>%s</td></tr></table></td>
                  <td><table cellpadding=5px cellspacing=0 align=right style='text-align: right; width:100%%;'><tr><td bgcolor=%s>%s</td></tr><tr><td bgcolor=%s>%s</td></tr><tr><td bgcolor=%s>%s</td></tr></table></td>
                  <td><table cellpadding=5px cellspacing=0 align=right style='text-align: right; width:100%%;'><tr><td bgcolor=%s>%s</td></tr><tr><td bgcolor=%s>%s</td></tr><tr><td bgcolor=%s>%s</td></tr></table></td>
                  <td><table cellpadding=5px cellspacing=0 align=right style='text-align: right; width:100%%;'><tr><td bgcolor=%s>%s</td></tr><tr><td bgcolor=%s>%s</td></tr><tr><td bgcolor=%s>%s</td></tr></table></td>
                </tr>
                ", fcolor($score,'score'), $val['loan_officer'], fcolor($score,'score'), $score,
                   'silver', $val['loans_3'], 'silver', $val['loans_2'], 'silver', $val['loans_1'],
                   'silver', $val['delinquent_loans_3'], 'silver', $val['delinquent_loans_2'], 'silver', $val['delinquent_loans_1'],
                   'silver', number_format($val['due_amount_3'],2,'.',','), 'silver', number_format($val['due_amount_2'],2,'.',','), 'silver', number_format($val['due_amount_1'],2,'.',','),
                   fcolor($val['clients_3'],'clients'), $val['clients_3'], fcolor($val['clients_2'],'clients'), $val['clients_2'], fcolor($val['clients_1'],'clients'), $val['clients_1'],
                   fcolor($val['women_pct_3'],'women_pct'), round($val['women_pct_3'],0), fcolor($val['women_pct_2'],'women_pct'), round($val['women_pct_2'],0), fcolor($val['women_pct_1'],'women_pct'), round($val['women_pct_1'],0),
                   fcolor($val['balance_3'],'balance'), number_format($val['balance_3'],2,'.',','), fcolor($val['balance_2'],'balance'), $val['balance_2'], fcolor($val['balance_1'],'balance'), number_format($val['balance_1'],2,'.',','),
                   fcolor($val['riskA_3'],'riskA'), $val['riskA_3'], fcolor($val['riskA_2'],'riskA'), $val['riskA_2'], fcolor($val['riskA_1'],'riskA'), $val['riskA_1'],
                   'silver', round(100*($val['write_off_3']/$val['balance_1']),2), 'silver', round(100*($val['write_off_2']/$val['balance_1']),2), fcolor(100*($val['write_off_1']/$val['balance_1']),'write_off_pct'), round(100*($val['write_off_1']/$val['balance_1']),2),
                   'silver', number_format($val['op_income_3'],2,'.',','), 'silver', number_format($val['op_income_2'],2,'.',','), fcolor($val['op_income_1'],'op_income'), number_format($val['op_income_1'],2,'.',','),
                   'silver', $val['formalizations_3'], 'silver', $val['formalizations_2'], fcolor($val['formalizations_1'],'formalizations'), $val['formalizations_1'],
                   'silver', number_format($val['disbursements_3'],2,'.',','), 'silver', number_format($val['disbursements_2'],2,'.',','), fcolor($val['disbursements_1'],'disbursements'), number_format($val['disbursements_1'],2,'.',','),
                   'silver', number_format($val['write_off_3'],2,'.',','), 'silver', number_format($val['write_off_2'],2,'.',','), 'silver', number_format($val['write_off_1'],2,'.',','),
                   'silver', number_format($val['kp_income_3'],2,'.',','), 'silver', number_format($val['kp_income_2'],2,'.',','), 'silver', number_format($val['kp_income_1'],2,'.',','));
}
$_html  .= "</table>";

// finishing message

$subject = sprintf('Desempeño de asesores de crédito: %s',WEBPAGE::$conf['app']['client_name']);
$subject = strip_tags($subject);
$message = $subject;
$attachment = $_html .'</body></html>';
$recipients = explode(';',WEBPAGE::$conf['app']['par30rpt_emailto']);
// Additional headers
$headers["From"] = 'e-MMS Reports <emms_reports@esperanza.org>';
$headers["Subject"] = $subject;
$crlf = "\n";
$mime = new Mail_mime($crlf);
$mime->setTxtBody($message);
$mime->setHTMLBody($attachment);
//do not ever try to call these lines in reverse order
$body = $mime->get();
$hdr = $mime->headers($headers,true);
$params["host"] = WEBPAGE::$conf['mail']['host'];
$params["port"] = WEBPAGE::$conf['mail']['port'];
$params["username"] = WEBPAGE::$conf['mail']['username'];
$params["password"] = WEBPAGE::$conf['mail']['password'];
$params["auth"] = true;
$mail =& Mail::factory('mail',$params);
$res = $mail->send($recipients, $hdr, $body);
//if (PEAR::isError($res)) echo 'error enviando el email';
if (PEAR::isError($res)) { print($res->getMessage());}

function fcolor($val, $par)
{
  if(WEBPAGE::$conf['app']['performance.officer.'.$par.'.A'])
  {
    if ($val >= WEBPAGE::$conf['app']['performance.officer.'.$par.'.A']) return WEBPAGE::$conf['app']['performance.color.A'];
    if ($val >= WEBPAGE::$conf['app']['performance.officer.'.$par.'.B']) return WEBPAGE::$conf['app']['performance.color.B'];
    if ($val >= WEBPAGE::$conf['app']['performance.officer.'.$par.'.C']) return WEBPAGE::$conf['app']['performance.color.C'];
    return WEBPAGE::$conf['app']['performance.color.D'];
  } else {
    if ($val >= WEBPAGE::$conf['app']['performance.officer.'.$par.'.D']) return WEBPAGE::$conf['app']['performance.color.D'];
    if ($val >= WEBPAGE::$conf['app']['performance.officer.'.$par.'.C']) return WEBPAGE::$conf['app']['performance.color.C'];
    if ($val >= WEBPAGE::$conf['app']['performance.officer.'.$par.'.B']) return WEBPAGE::$conf['app']['performance.color.B'];
    return WEBPAGE::$conf['app']['performance.color.A'];
  }
}

function fscore($val, $par)
{
  if(WEBPAGE::$conf['app']['performance.officer.'.$par.'.A'])
  {
    if ($val >= WEBPAGE::$conf['app']['performance.officer.'.$par.'.A']) return WEBPAGE::$conf['app']['performance.officer.'.$par.'.weight'];
    if ($val >= WEBPAGE::$conf['app']['performance.officer.'.$par.'.B']) return WEBPAGE::$conf['app']['performance.officer.'.$par.'.weight']/2;
    if ($val >= WEBPAGE::$conf['app']['performance.officer.'.$par.'.C']) return WEBPAGE::$conf['app']['performance.officer.'.$par.'.weight']/4;
    return 0;
  } else {
    if ($val >= WEBPAGE::$conf['app']['performance.officer.'.$par.'.D']) return 0;
    if ($val >= WEBPAGE::$conf['app']['performance.officer.'.$par.'.C']) return WEBPAGE::$conf['app']['performance.officer.'.$par.'.weight']/4;
    if ($val >= WEBPAGE::$conf['app']['performance.officer.'.$par.'.B']) return WEBPAGE::$conf['app']['performance.officer.'.$par.'.weight']/2;
    return WEBPAGE::$conf['app']['performance.officer.'.$par.'.weight'];
  }
}
?>
