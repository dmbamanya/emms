<?php
$date_from 	= $_REQUEST['date_from'] 	? $_REQUEST['date_from'] 	: date('Y-m-d');
$date_to 	= $_REQUEST['date_to'] 		? $_REQUEST['date_to'] 		: date('Y-m-d');

$data = WEBPAGE::$dbh->getAll(sprintf("
								select
                                    xl.loan_code, c.code, concat(c.last,', ', c.first) client, if((lm.borrower_type != 'I'),s.name,'-') bde, xl.kp, u.username, f.fund, p.program, z.zone, xl.date_g, xl.date_c
                                from
                                    (
                                    select
                                        lsh.loan_id, l.delivered_date date_g, lsh.date date_c, l.client_id, l.loan_code, l.program_id, l.zone_id, lmd.master_id, l.advisor_id, l.kp
                                    from
                                        tblLoans l, tblLoanStatusHistory lsh, tblLoansMasterDetails lmd
                                    where
                                        lsh.status = 'C' and
                                        lsh.date >= '%s' and
                                        lsh.date <= '%s' and
                                        l.id = lsh.loan_id and
                                        lmd.loan_id = lsh.loan_id
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
                                    f.id = flmp.fund_id", $date_from, $date_to));

$head = array('loan_code' => WEBPAGE::$gt['tblLoans.loan_code'], 'code' => WEBPAGE::$gt['tblClients.code'], 'client' => WEBPAGE::$gt['tblClients.id'], 'bde' => WEBPAGE::$gt['tblClients.society_id'], 'kp' => WEBPAGE::$gt['tblLoans.kp'],'username' => WEBPAGE::$gt['tblClients.advisor_id'], 'fund' => WEBPAGE::$gt['tblFunds.fund'], 'program' => WEBPAGE::$gt['tblPrograms.program'], 'zone' => WEBPAGE::$gt['tblZones.zone'], 'date_g' => WEBPAGE::$gt['tblLoans.delivered_date'], 'date_c' => WEBPAGE::$gt['cancel_date']);

$tpl->setVariable('rpt_label', 			WEBPAGE::$gt['cancelledLoanPortfolio']);
$tpl->setVariable('rpt_subtitle_label', WEBPAGE::$gt['RP.SCR.cancelledLoanPortfolioByLoan']);
$tpl->setVariable('date_range_label', 	WEBPAGE::$gt['dateRange']);
$tpl->setVariable('date_range', 		sprintf('%s : %s',$date_from,$date_to));
$tpl->setVariable('chart', count($data) ? WEBPAGE::printchart($data,$head) : WEBPAGE::$gt['noData']);

$tpl->setVariable('cancelledInPeriod', 	WEBPAGE::$gt['cancelledInPeriod']);

?>
