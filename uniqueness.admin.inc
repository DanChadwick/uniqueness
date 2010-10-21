<?php
// $Id$

/**
 * @file
 * Settings page for the uniqueness module.
 */

/**
 * Form builder for uniqueness settings page; system_settings_form().
 */
function uniqueness_settings() {
  $form = array();

  // Search options.
  $form['search'] = array(
    '#type' => 'fieldset',
    '#title' => t('Search'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['search']['uniqueness_search_mode'] = array(
    '#type' => 'radios',
    '#title' => t('Search mode'),
    '#description' => t('Select the mode which should be used for generating the list of related nodes.'),
    '#options' => _uniqueness_search_mode_options(),
    '#default_value' => variable_get('uniqueness_search_mode', UNIQUENESS_SEARCH_MODE_NODETITLE),
    '#required' => TRUE,
  );
  $form['search']['uniqueness_scope'] = array(
    '#type' => 'radios',
    '#title' => t('Search scope'),
    '#options' => array(
      UNIQUENESS_SCOPE_ALL => t('Search in all nodes'),
      UNIQUENESS_SCOPE_CONTENT_TYPE => t('Search only within the content type of the node being added or edited.'),
    ),
    '#default_value' => variable_get('uniqueness_scope', UNIQUENESS_SCOPE_ALL),
    '#description' => t('Search all nodes or just nodes of the same content type.'),
    '#required' => TRUE,
  );
  $form['search']['uniqueness_results_prepend'] = array(
    '#type' => 'radios',
    '#title' => t('Results display'),
    '#options' => array(
      0 => t('Replace old results with new ones'),
      1 => t('Leave old results and prepend new ones'),
    ),
    '#default_value' => variable_get('uniqueness_results_prepend', 0),
    '#description' => t('Choose if new results replace or are added to existing results. Browser cache may keep this setting from taking affect right away.'),
    '#required' => TRUE,
  );
  $form['search']['uniqueness_maxresults'] = array(
    '#type' => 'textfield',
    '#title' => t('Maximum number of results'),
    '#default_value' => variable_get('uniqueness_maxresults', 10),
    '#size' => 5,
    '#maxlength' => 4,
    '#element_validate' => array('uniqueness_form_validate_maxresults'),
    '#description' => t('Limit the number of search results to this number of fewer. (If using "Drupal search,", must be 10 or fewer.)'),
    '#required' => TRUE,
  );

  // Appearance.
  $form['appearance'] = array(
    '#type' => 'fieldset',
    '#title' => t('Appearance'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['appearance']['uniqueness_widgets'] = array(
    '#type' => 'checkboxes',
    '#options' => array(
      UNIQUENESS_WIDGET_INLINE => t('Display related content inline, embedded on the "node add" form.'),
      UNIQUENESS_WIDGET_BLOCK => t('Provide a block for displaying related content.'),
    ),
    '#default_value' => variable_get('uniqueness_widgets', array(UNIQUENESS_WIDGET_INLINE)),
  );
  $form['appearance']['uniqueness_default_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Default title'),
    '#default_value' => variable_get('uniqueness_default_title', t('Related content')),
    '#description' => t('Note: when using the widget as a block then this title can be overriden by the block title on the !block_settings_page.',
        array('!block_settings_page' => l(t('uniqueness block settings page'), 'admin/build/block/configure/uniqueness/uniqueness'))),
    '#required' => TRUE,
  );
  $form['appearance']['uniqueness_default_description'] = array(
    '#type' => 'textarea',
    '#title' => t('Default description'),
    '#default_value' => variable_get('uniqueness_default_description',
        t("Help us increase the signal to noise ratio! If we find content that's related or similar to what you're posting it will be listed here.")),
    '#rows' => 2,
    '#description' => t('Heading text displayed within the widget before the list of duplicates.'),
  );
  $form['appearance']['uniqueness_page_visibility'] = array(
    '#type' => 'textarea',
    '#title' => t('Page visibility'),
    '#default_value' => variable_get('uniqueness_page_visibility', "node/add/*"),
    '#rows' => 2,
    '#description' => t('Configure which "node add" or "node edit" pages should display related content. Enter one page per line as Drupal paths. ' .
                        'The "*" characters is a wildcard. Example paths are %add for adding new content and %edit for editing existing content.',
                        array('%add' => 'node/add/*', '%edit' => 'node/*/edit')),
  );

  return system_settings_form($form);
}

function _uniqueness_search_mode_options() {
  $options = array(
    UNIQUENESS_SEARCH_MODE_NODETITLE => t('Simple node title search'),
  );
  if (module_exists('search')) {
    $options += array(
      UNIQUENESS_SEARCH_MODE_DRUPAL => t('Drupal search'),
    );
  }
  if (module_exists('apachesolr_search')) {
    $options += array(
      UNIQUENESS_SEARCH_MODE_SOLR => t('Apache Solr Search'),
    );
  }
  return $options;
}

/**
 * Implementation of hook_form_validate().
 */
function uniqueness_form_validate_maxresults($element, &$form_state) {
 // Validate that the number of results to show is numeric and within range
 $upper_limit = ($form_state['values']['uniqueness_search_mode'] == UNIQUENESS_SEARCH_MODE_DRUPAL) ? 10 : 100;
 $value = $element['#value'];
  if (!is_numeric($value) || $value <= 0 || $value > $upper_limit) {
    form_error($element, t('The number of results must between 1 and !upper.', array('!upper' => $upper_limit)));
  }
}
