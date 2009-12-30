<?php
class PROGRAM extends WEBPAGE
{  
  /* FUND parameters */
  var $frostdata = array('confidential');
  var $programdata  = array( 'status' => 'A', 'creator_id' => 0);
  var $programfields = array();

  function PROGRAM($id = '')  //class constructor
  {
  if ($id) {

    $this->programdata = current(SQL::select('tblPrograms','*',sprintf('id=%s',$id)));

    } else {

    $this->programdata['creator_date'] = date('Y-m-d');
    $this->frostdata = array();  

    }
  }

  function getTemplateData($id)
  {
  $fields = sprintf("
  				p.*, 
		    	CONCAT(uc.first,' ',uc.last) AS creator, 
		    	DATE_FORMAT(p.creator_date,'%s') AS f_creator_date,
		    	CONCAT(ue.first,' ',ue.last) AS editor,  
		    	DATE_FORMAT(p.editor_date,'%s') AS f_editor_date",WEBPAGE::$gt['date_format_mysql'],WEBPAGE::$gt['date_format_mysql']);
  $tables = 'tblPrograms AS p, tblUsers AS uc, tblUsers AS ue';
  $param  = sprintf("p.id = '%s' AND p.creator_id = uc.id AND p.editor_id = ue.id", $id);

  $data 			= current(SQL::select($tables, $fields, $param));

  $c=0;
  $data['buttondata'][$c][id]	    = "BS.SCR.addProgram";
  $data['buttondata'][$c][href]		= "index.php?scr_name=BS.SCR.addProgram&id=".$id;
  $data['buttondata'][$c][alt]		= WEBPAGE::$gt['edit'];
  $data['buttondata'][$c][onClick]	= "";
  $data['buttondata'][$c][ico]		= "edit";

  return $data;
  }
  
  function getFunds($id = '')
  {
  if (!($id)) {$id = $this->programdata['id'];}
  $fields = 'l.fund_id AS id, f.fund'; 
  $table = 'tblLinkProgramsFunds AS l, tblFunds AS f';
  $param = sprintf("l.program_id = '%s' AND f.id = l.fund_id",$id);  
  return SQL::getAssoc($table,$fields,$param);
  }

  function funds()
  {
  $fields = 'id, fund'; 
  $tables = 'tblFunds';
  $left = 'tblLinkProgramsFunds'; 
  $on = sprintf("id = fund_id AND program_id = '%s'",$this->programdata['id']);
  $param = "fund_id IS NULL AND status = 'A'";
  
  return (SQL::getAssoc_leftjoin($tables, $fields, $left, $on, $param));
  }

  function getAll($status = '')
  {
  $status_operator = '=';
  if (!($status)) {$status_operator = 'like'; $status = '%%';}
  $fields = 'id, program';
  $table = 'tblPrograms';
  $param = sprintf("status %s '%s'",$status_operator,$status);
  return SQL::getAssoc($table,$fields,$param);
  }
}
?>