<?php

// Nothing but helper formatters for Views.

function url_thumbnail_views_data_alter(&$data) {
  $data['users']['table']['join']['twitter'] = array(
    'left_table' => 'twitter_user',
    'left_field' => 'uid',
    'field' => 'uid',
  );

  foreach($data as $table => $tabledata) {
    foreach ($tabledata as $fieldname => $field) {
      if ($fieldname != 'table') {
        if (!empty($field['field']['handler']) && $field['field']['handler'] == 'views_handler_field_url') {
          $data[$table][$fieldname]['field']['handler'] = 'url_thumbnailer_views_handler_field_url';
        }
      }
    }
  }
}

/**
 * Field handler to provide simple renderer that turns a URL into a clickable link.
 */
class url_thumbnailer_views_handler_field_url extends views_handler_field_url {
  function option_definition() {
    $options = parent::option_definition();

    $options['display_as_thumbnail'] = array('default' => FALSE);
    $options['thumbnail_size'] = array('default' => 's');

    return $options;
  }

  /**
   * Provide link to the page being visited.
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['display_as_thumbnail'] = array(
      '#title' => t('Display as thumbnail'),
      '#type' => 'checkbox',
      '#default_value' => !empty($this->options['display_as_thumbnail']),
    );
    $form['thumbnail_size'] = array(
      '#title' => t('Thumbnail size'),
      '#type' => 'select',
      '#options' => _url_thumbnail_sizes(),
      '#default_value' => $this->options['thumbnail_size'],
    );
  }

  function render($values) {
    $url = $values->{$this->field_alias};
    $link = !empty($this->options['display_as_link']);
    $thumb = !empty($this->options['display_as_thumbnail']);
    $size = $this->options['thumbnail_size'];
    
    if ($thumb) {
      return theme('url_thumbnail', $url, $size, $link);
    }
    elseif ($link) {
      return l(check_plain($url), $url, array('html' => TRUE));
    }
    else {
      return $url;
    }
  }
}
