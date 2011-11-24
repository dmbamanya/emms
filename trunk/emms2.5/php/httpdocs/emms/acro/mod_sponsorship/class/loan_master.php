<?php
class LOAN_MASTER extends WEBPAGE
{
  /* LOAN_MASTER parameters */
  var $frostdata = array();
  var $data  = array();
  var $fields = array();

  function LOAN_MASTER($id = '')  //class constructor
  {
  if ($id) {

    $this->data = current(SQL::select('tblLoansMaster','*',sprintf('id=%s',$id)));
    $this->loadloans();

    } else {

    $this->data['check_status'] = 'P';
    $this->data['creator_date'] = date('Y-m-d');
    $this->data['editor_date']  = date('Y-m-d');
    $this->data['creator_id']   = WEBPAGE::$userID;
    $this->data['editor_id']    = WEBPAGE::$userID;

    }
  }

  function updateAmount($id)
  {
  WEBPAGE::$dbh->query(sprintf("update tblLoansMaster as lm set lm.amount = (select sum(l.kp) from tblLoans as l, tblLoansMasterDetails as lmd where lmd.master_id = %s and lmd.loan_id = l.id) where lm.id = %s",$id,$id));
  }

  function updateCheckNumber()
  {
  WEBPAGE::$dbh->query(sprintf("update tblLoansMaster as lm set lm.check_number = '%s' where lm.id = %s",$this->data['check_number'],$this->data['id']));
  }

  function getTemplateData($id)
  {
  /*
  $fields = sprintf("
  				f.*, c.currency,
		    	CONCAT(uc.first,' ',uc.last) AS creator,
		    	DATE_FORMAT(f.creator_date,'%s') AS f_creator_date,
		    	CONCAT(ue.first,' ',ue.last) AS editor,
		    	DATE_FORMAT(f.editor_date,'%s') AS f_editor_date", WEBPAGE::$gt['date_format_mysql'], WEBPAGE::$gt['date_format_mysql']);
  $tables = "tblFunds AS f, tblCurrencys AS c, tblUsers AS uc, tblUsers AS ue";
  $param  = sprintf("f.id = '%s' AND c.id = f.currency_id AND f.creator_id = uc.id AND f.editor_id = ue.id",$id);

  $data 				  = current(SQL::select($tables, $fields, $param));

  $c=0;
  $data['buttondata'][$c][id]	    = "AC.SCR.addFund";
  $data['buttondata'][$c][href]		= "index.php?scr_name=AC.SCR.addFund&id=".$id;
  $data['buttondata'][$c][alt]		= WEBPAGE::$gt['edit'];
  $data['buttondata'][$c][onClick]	= "";
  $data['buttondata'][$c][ico]		= "edit";

  return $data;
  */
  }

  function check_status()
  {
  $opt = explode(",", WEBPAGE::$gt['tblLoansMaster.check_status.optList']);
  $nam = explode(",", WEBPAGE::$gt['tblLoansMaster.check_status.optNames']);
  foreach ($opt as $key => $val) {
    $ls[$val] = $nam[$key];
    }
  return $ls;
  }

  function get_duplicates()
  {
  if (WEBPAGE::$conf['app']['duplicate_chk_request_margin'] < 0) { return array(); }
  return WEBPAGE::$dbh->getAssoc(sprintf("select
                                        id,id
                                    from
                                        tblLoansMaster
                                    where
                                        id != '%s' and
                                        borrower_id = '%s' and
                                        borrower_type = '%s' and
                                        abs(datediff(creator_date,'%s')) <= '%s'",
                                        $this->data['id'],
                                        $this->data['borrower_id'],
                                        $this->data['borrower_type'],
                                        $this->data['creator_date'],
                                        WEBPAGE::$conf['app']['duplicate_chk_request_margin']));
  }
  
  function loadloans()
  {
  $this->data['loans'] = SQL::getAssoc('tblLoansMasterDetails AS lmd, tblLoans AS l', 'l.id, l.status', sprintf('lmd.master_id = %s AND l.id = lmd.loan_id', $this->data['id']));
  foreach($this->data['loans'] as $status) {
    $this->data['cancelled'] = false;
    if ($status != 'C') { break; } else { $this->data['cancelled'] = true; }
    }
  }

  function loadextrainfo()
  {
  $this->data['extrainfo'] = WEBPAGE::$dbh->getAssoc(sprintf("select l.id,l.kp,l.pmt,lt.payment_frequency,concat(c.first,' ',c.last) client_name,c.code client_code,c.address client_address from tblLoans l, tblClients c, tblLoanTypes lt where l.id in (select lmd.loan_id id from tblLoansMasterDetails lmd where lmd.master_id = %s) and c.id = l.client_id and lt.id = l.loan_type_id", $this->data['id']));
  }

  function load_pmt_receipt_flag_a()
  {
  $this->data['pmt_receipt_flag_a'] = WEBPAGE::$dbh->getAssoc(sprintf("select p.loan_id, count(p.id) from tblPayments p, tblReceipts r, tblLinkReceiptsPayments rp where p.loan_id in (select lmd.loan_id from tblLoansMasterDetails lmd where lmd.master_id = %s) and  r.flag_a = '1' and r.id = rp.receipt_id and rp.payment_id = p.id group by p.loan_id", $this->data['id']));
  }

  function load_pmt_receipt_flag_b()
  {
  $this->data['pmt_receipt_flag_b'] = WEBPAGE::$dbh->getAssoc(sprintf("select p.loan_id, count(p.id) from tblPayments p, tblReceipts r, tblLinkReceiptsPayments rp where p.loan_id in (select lmd.loan_id from tblLoansMasterDetails lmd where lmd.master_id = %s) and  r.flag_b = '1' and r.id = rp.receipt_id and rp.payment_id = p.id group by p.loan_id", $this->data['id']));
  }

  function loadpayments()
  {
  $this->data['payments'] = WEBPAGE::$dbh->getAssoc(sprintf("SELECT p.loan_id, concat(c.first,' ',c.last) client, sum(p.pmt) pmt, sum(p.principal) principal, sum(p.interest) interest, sum(p.fees) fees, sum(p.insurances) insurances, sum(p.penalties) penalties FROM tblClients c, tblLoans l, tblPayments p, tblLoansMasterDetails lmd where c.id = l.client_id and l.id = lmd.loan_id and p.loan_id = lmd.loan_id and lmd.master_id = %s group by p.loan_id;", $this->data['id']));
  }

  function loadBorrowerData()
  {
  switch ($this->data['borrower_type']) {
    case 'B':
//      require_once 'class/society.php';
      $this->data['borrower'] = new SOCIETY($this->data['borrower_id']);
      break;
    case 'G':
//      require_once 'class/society.php';
      $this->data['borrower'] = new SOCIETY($this->data['borrower_id']);
      break;
    case 'I':
//      require_once 'class/client.php';
      $this->data['borrower'] = new CLIENT($this->data['borrower_id']);
      break;
    default:
      WEBPAGE::redirect('index.php?logout=1');
      exit;
    }
  }

/*
 * Retrive all master_ids that apply for donate publishing
 * that is, disbursed within the last WEBPAGE::$conf['mod_sponsorship']['publishing_margin'] days
 */
  static function publishable_ids()
  {
    $xrate = current(current(WEBPAGE::$dbh->getAll(sprintf
            (
              "select rate from tblCurrenciesExchangeRates where date = '%s' and currency_id = %s", 
               date('Y-m-d'),WEBPAGE::$conf['app']['xrate_local']))));
    $margin = $xrate * WEBPAGE::$conf['mod_sponsorship']['min_donation'];
    return WEBPAGE::$dbh->getAssoc(sprintf
    (
     "select distinct
        lm.id, 1 + %s - datediff(curdate(),ckd.date) time_left,
        coalesce(rc.donations,0.00)                  donations,
        br.borrowers                                 borrowers,
        lm.amount - coalesce(rc.donations,0.00)      pending
      from
      (
        tblLoansMaster lm,
        (
          select
            lmd.master_id, lsh.date
          from
            tblLoanStatusHistory lsh,
            tblLoansMasterDetails lmd
          where
            lmd.loan_id = lsh.loan_id and
            lsh.status = 'G'          and
            lsh.date < CURDATE()   and
            lsh.date >= DATE_ADD(CURDATE(), INTERVAL -%s DAY)
        ) ckd
      )
      left join
      (
        select master_id,sum(donation) donations from tblLoansMasterSponsors group by master_id
      ) rc on rc.master_id = lm.id
      left join
      (
        select master_id,count(loan_id) borrowers from tblLoansMasterDetails lmd group by master_id
      ) br on br.master_id = lm.id
      where
        lm.borrower_type = 'B'   and
        lm.check_status  = 'R'   and
        ckd.master_id    = lm.id and
        br.borrowers >= %s        and
        br.borrowers <= %s        and
        coalesce(rc.donations,0.00) < lm.amount - %s
      order by
        lm.id",
      WEBPAGE::$conf['mod_sponsorship']['publishing_margin'],
      WEBPAGE::$conf['mod_sponsorship']['publishing_margin'],
      WEBPAGE::$conf['mod_sponsorship']['publishing_min_borrowers'],
      WEBPAGE::$conf['mod_sponsorship']['publishing_max_borrowers'],
      $margin)
    );
  }

  /*
   * Pre-register a new donation.
   * Check if master_id is sold out, if so then rollback and return false
   */
  function add_donation($donation,$tip,$token)
  {
      WEBPAGE::$dbh->query(sprintf("insert into tblLoansMasterSponsors (id,master_id,donation,tip,datetime,token) values ('null',%s,'%s','%s','%s','%s')",
              $this->data['id'], $donation, $tip, gmdate("Y-m-d H:i:s", time()), $token));
      $rised = current(current(WEBPAGE::$dbh->getAll(sprintf("select sum(donation) from tblLoansMasterSponsors where master_id = %s",$this->data['id']))));
      if ($rised > $this->data['amount'])
      {
          WEBPAGE::$dbh->query(sprintf("delete from tblLoansMasterSponsors where token = '%s'", $token));
          return false;
      }
      return true;
  }

  function commit_donation($token,$sponsor_id)
  {
      WEBPAGE::$dbh->query(sprintf("update tblLoansMasterSponsors set sponsor_id = %s where token = '%s' and master_id = %s", $sponsor_id, $token, $this->data['id']));
  }

  function isSponsor($sponsor_id)
  {
      return current(current(WEBPAGE::$dbh->getAll(sprintf("select count(id) hits from tblLoansMasterSponsors where master_id = %s and sponsor_id = %s", $this->data['id'], $sponsor_id))));
  }

  function getBorrowerZone()
  {
    return  current(WEBPAGE::$dbh->getAssoc(sprintf("select id,id from tblZones where parent_id = %s and program_id = %s", $this->data['zone_id'],$this->data['program_id'])));
  }
}
?>
