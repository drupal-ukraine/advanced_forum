function jq_collapse_state () {
	return false;
}

function jq_collapse (container_id) {
	$(document).ready(function(){
		$('.in-container-' + container_id).toggle();
  	$.get(Drupal.settings.advanced_forum_ajax + '/toggle/' + container_id, null, function(data) {});
  	
  return false;
	});
}

$(document).ready(function(){
  $.getJSON(Drupal.settings.advanced_forum_ajax + '/load', function(data) {
    $.each(data.tids, function(tid, hide){
    if(hide) {
      $('.in-container-' + tid).toggle();
    }
    });
  });
    
  return false;
});