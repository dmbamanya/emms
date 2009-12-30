<?php
class BUSINESS_TYPE extends WEBPAGE
{  
  /* BUSINESS_TYPE parameters */
  var $frostdata 	= array();
  var $data  		= array('creator_id' => 0);
  var $btypefields  = array();

  function BUSINESS_TYPE($id = '')  //class constructor
  {
  if ($id) {

    $this->data = current(SQL::select('tblBusinessTypes','*',sprintf('id=%s',$id)));
    
    } else {

    $this->data['creator_date'] = date('Y-m-d');

    }
  }

  function getTemplateData($id)
  {
  $btype = new BUSINESS_TYPE($id);
  return $btype->data;
  }
  
  function activities()
  {
  $opt = explode(",", WEBPAGE::$gt['tblBusinessTypes.activity.optList']);
  $nam = explode(",", WEBPAGE::$gt['tblBusinessTypes.activity.optNames']);
  foreach ($opt as $key => $val) {
    $bt[$val] = $nam[$key];
    }
  return $bt;
  }
  
}
?>